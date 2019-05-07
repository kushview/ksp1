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

#include "engine/LayerData.h"
#include "engine/SampleCache.h"
#include "engine/SamplerKeys.h"

#define KSP1_SAMPLE_CACHE_TIMEOUT 50

namespace KSP1 {

SampleCache::SampleCache (AudioFormatManager& f, AudioThumbnailCache& c)
    : formats (f), thumbs (c)
{ }

SampleCache::~SampleCache()
{

}

void SampleCache::activate (double samplerate, uint32 buffersize)
{
    ignoreUnused (samplerate, buffersize);

    while (threads.size() <= 4)
    {
        String name = "sc";
        name << threads.size() + 1;
        threads.add (new TimeSliceThread (name));
    }

    for (auto* const t : threads)
        t->startThread (threadPriority);
}


void SampleCache::deacitvate()
{
    for (TimeSliceThread* t : threads)
        t->stopThread (KSP1_SAMPLE_CACHE_TIMEOUT);
}

bool SampleCache::canLoad (const File& file) const
{
    return ! file.isDirectory() && file.existsAsFile();
}

#if 0
AudioPeak* SampleCache::createPeak (const File& file)
{
    if (AudioPeak* peak = new AudioPeak (*this))
    {
        peak->setSource (new FileInputSource (file));
        return peak;
    }

    return nullptr;
}
#endif

LayerData* SampleCache::createLayerSource (const XmlElement& item)
{
    LayerData* source = nullptr;

    if (Scoped<AudioFormatReader> reader = createReaderFor (item))
    {
        source = new LayerData (*this, item.getIntAttribute ("id", 0));
        source->index = item.getIntAttribute ("index", -1);
        source->note = item.getIntAttribute ("note", -1);
        source->numChannels = reader->numChannels;
        source->sampleRate = reader->sampleRate;
        source->lengthInSamples = reader->lengthInSamples;
        source->pitch.set (item.getDoubleAttribute ("pitch"));
    }
    else
    {
        jassertfalse;
    }

    return source;
}


AudioFormatReader* SampleCache::createReaderFor (const XmlElement& item)
{
    if (item.hasAttribute ("file"))
    {
        const auto path = item.getStringAttribute ("file", "");
        return File::isAbsolutePath (path) ? createReaderFor (File (path)) : nullptr;
    }
    else if (auto* e = item.getChildByName ("file"))
    {
        const auto path = e->getAllSubText();
        return File::isAbsolutePath (path) ? createReaderFor (File (path)) : nullptr;
    }

    return nullptr;
}

AudioFormatReader* SampleCache::createReaderFor (const File& file)
{
    return getAudioFormats().createReaderFor (file);
}

BufferPtr SampleCache::loadAudioFile (const File& sampleFile)
{
    DBG ("[KSP1] loading: " << sampleFile.getFileName());

    const int32 slot (sampleFile.hashCode());

    if (audioBuffers.contains (slot))
        return audioBuffers [slot];

    BufferPtr buffer;

    if (blacklist.contains (slot))
        return buffer;

    if (auto reader = std::unique_ptr<AudioFormatReader> (getAudioFormats().createReaderFor (sampleFile)))
    {
        buffer.reset (new AudioSampleBuffer (reader->numChannels, reader->lengthInSamples));
        reader->read (buffer.get(), 0, reader->lengthInSamples, 0, false, false);
        audioBuffers.set (slot, buffer);
        reader.reset();
    }
    else
    {
        blacklist.add (slot);
    }

    return buffer;
}

TimeSliceThread* SampleCache::nextThread()
{
    assert (threads.size() > 0);
    return threads.getUnchecked(0);
}

LayerData* SampleCache::findLayerData (int32 id) const
{
    for (LayerData* layer : layers)
    {
        if (layer && layer->id == id)
            return layer;
    }

    return nullptr;
}

LayerData* SampleCache::getLayerData (const bool createIfNeeded)
{
    for (auto* const data : layers) {
        if (data && data->note < 0 && data->index < 0)
            return data;
    }

    return (createIfNeeded) ? layers.add (new LayerData (*this, 0))
                            : nullptr;
}

#if defined (HAVE_LVTK)
LayerData* SampleCache::getLayerData (const URIs& uris, const lvtk::AtomObject& layer, bool realtime)
{
    const int layerId = static_cast<int> (layer.id());
    LayerData* data = nullptr; //getLayerData(); //findLayerData (layer.id());

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

}
