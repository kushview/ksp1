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

#define SAMPLE_CACHE_TIMEOUT 50

namespace KSP1 {

    SampleCache::SampleCache()
#if 0
        : thumbs (128)
#endif
    {
        formats.registerBasicFormats();
        setThreadPriority (3);
    }

    SampleCache::~SampleCache()
    {

    }

    void SampleCache::activate (double samplerate, uint32 buffersize)
    {
        while (threads.size() <= 4) {
            String name = "SampleCache-";
            name << threads.size() + 1;
            threads.add (new TimeSliceThread (name));
        }

        for (TimeSliceThread* t : threads) {
            t->startThread (threadPriority);
        }
    }


    void SampleCache::deacitvate()
    {
        for (TimeSliceThread* t : threads) {
            t->stopThread (SAMPLE_CACHE_TIMEOUT);
        }
    }

    bool
    SampleCache::canLoad (const File& file) const
    {
        return ! file.isDirectory() && file.existsAsFile();
    }

#if 0
    AudioPeak*
    SampleCache::createPeak (const File& file)
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


    AudioFormatReader*
    SampleCache::createReaderFor (const XmlElement& item)
    {
        if (item.hasAttribute ("file"))
        {
            const File file (item.getStringAttribute ("file", File::nonexistent.getFullPathName()));
            return createReaderFor (file);
        }
        else if (XmlElement* e = item.getChildByName("file"))
        {
            const File file (e->getAllSubText());
            return createReaderFor (file);
        }

        return nullptr;
    }

    AudioFormatReader*
    SampleCache::createReaderFor (const File& file)
    {
        return getAudioFormats().createReaderFor (file);
    }


    Shared<AudioSampleBuffer>
    SampleCache::loadAudioFile (const File& sampleFile)
    {
        DBG ("Loading: " << sampleFile.getFileName());

        const int32 slot (sampleFile.hashCode());

        if (audioBuffers.contains (slot))
            return audioBuffers [slot];

        Shared<AudioSampleBuffer> buffer;

        if (blacklist.contains (slot))
            return buffer;

        if (AudioFormatReader* reader = getAudioFormats().createReaderFor (sampleFile))
        {
            buffer.reset (new AudioSampleBuffer (reader->numChannels, reader->lengthInSamples));
            reader->read (buffer.get(), 0, reader->lengthInSamples, 0, false, false);
            audioBuffers.set (slot, buffer);
            delete reader;
        }
        else
        {
            blacklist.add (slot);
        }

        return buffer;
    }

    TimeSliceThread*
    SampleCache::nextThread()
    {
        assert (threads.size() > 0);
        return threads.getUnchecked(0);
    }

    LayerData* SampleCache::findLayerData (int32 id) const {
        for (LayerData* l : layers) {
            if (l && l->id == id)
                return l;
        }
        return nullptr;
    }

    LayerData* SampleCache::getLayerData (const bool createIfNeeded)
    {
        for (LayerData* l : layers) {
            if (l && l->note < 0 && l->index < 0)
                return l;
        }

        return (createIfNeeded) ? layers.add (new LayerData (*this, 0)) : nullptr;
    }

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
}
