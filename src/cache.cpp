/*
    This file is part of KSP1
    Copyright (C) 2014  Kushview, LLC. All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "layerdata.hpp"
#include "samplerkeys.hpp"

#include "cache.hpp"

#define KSP1_SAMPLE_CACHE_TIMEOUT 50

namespace ksp1 {

SampleCache::SampleCache (FormatManager& f)
    : formats (f) {}

SampleCache::~SampleCache() {}

void SampleCache::activate (double samplerate, int buffersize) {
    juce::ignoreUnused (samplerate, buffersize);
}

void SampleCache::deacitvate() {
}

bool SampleCache::canLoad (const juce::File& file) const {
    return ! file.isDirectory() && file.existsAsFile();
}

LayerData* SampleCache::createLayerSource (const juce::XmlElement& item) {
    LayerData* source = nullptr;

    if (auto reader = createReaderFor (item)) {
        source                  = new LayerData (*this, item.getIntAttribute ("id", 0));
        source->index           = item.getIntAttribute ("index", -1);
        source->note            = item.getIntAttribute ("note", -1);
        source->numChannels     = reader->numChannels;
        source->sampleRate      = reader->sampleRate;
        source->lengthInSamples = reader->lengthInSamples;
        source->pitch.set (item.getDoubleAttribute ("pitch"));
    } else {
        jassertfalse;
    }

    return source;
}

FormatReader* SampleCache::createReaderFor (const juce::XmlElement& item) {
    if (item.hasAttribute ("file")) {
        const auto path = item.getStringAttribute ("file", "");
        return juce::File::isAbsolutePath (path) ? createReaderFor (juce::File (path)) : nullptr;
    } else if (auto* e = item.getChildByName ("file")) {
        const auto path = e->getAllSubText();
        return juce::File::isAbsolutePath (path) ? createReaderFor (juce::File (path)) : nullptr;
    }

    return nullptr;
}

FormatReader* SampleCache::createReaderFor (const juce::File& file) {
    return getAudioFormats().createReaderFor (file);
}

BufferPtr SampleCache::loadAudioFile (const juce::File& sampleFile) {
    const int slot (sampleFile.hashCode());
    if (audioBuffers.contains (slot))
        return audioBuffers[slot];

    BufferPtr buffer;

    if (blacklist.contains (slot))
        return buffer;

    if (auto reader = std::unique_ptr<FormatReader> (getAudioFormats().createReaderFor (sampleFile))) {
        DBG ("[ksp1] loading: " << sampleFile.getFileName());
        buffer.reset (new SampleBuffer ((int) reader->numChannels, reader->lengthInSamples));
        reader->read (buffer.get(), 0, reader->lengthInSamples, 0, false, false);
        audioBuffers.set (slot, buffer);
        reader.reset();
    } else {
        DBG ("[KSP1] could not load: " << sampleFile.getFileName());
        blacklist.add (slot);
    }

    return buffer;
}

LayerData* SampleCache::findLayerData (const int layerId) const {
    for (LayerData* layer : layers) {
        if (layer && layer->id == layerId)
            return layer;
    }

    return nullptr;
}

juce::File SampleCache::resolvePath (const juce::String& path) const noexcept {
    if (juce::File::isAbsolutePath (path))
        return { path };
    for (int i = 0; i < _searchPath.getNumPaths(); ++i) {
        auto child = _searchPath[i].getChildFile (path);
        if (child.existsAsFile())
            return child;
    }
    return {};
}

LayerData* SampleCache::getLayerData (const bool createIfNeeded) {
    for (auto* const data : layers) {
        if (data && data->note < 0 && data->index < 0)
            return data;
    }

    return (createIfNeeded) ? layers.add (new LayerData (*this, 0))
                            : nullptr;
}

#if defined(HAVE_LVTK)
LayerData* SampleCache::getLayerData (const URIs& uris, const lvtk::AtomObject& layer, bool realtime) {
    const int layerId = static_cast<int> (layer.id());
    LayerData* data   = nullptr; //getLayerData(); //findLayerData (layer.id());

    if (! data) {
        for (LayerData* l : layers) {
            if (l && l->id == layerId) {
                data = l;
                break;
            }
        }
    }

    if (! data && ! realtime) {
        data = layers.add (new LayerData (*this, layerId));
    }

    if (data) {
        data->setAtomObject (uris, layer, realtime);
    }

    return data;
}
#endif

} // namespace ksp1
