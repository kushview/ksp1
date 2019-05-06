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

#pragma once

#include "KSP1.h"

namespace KSP1 {

class LayerData;
typedef std::shared_ptr<AudioSampleBuffer> BufferPtr;

class SampleCache
{
public:
    SampleCache (AudioFormatManager& f);
    ~SampleCache();

    LayerData* getLayerData (const bool createIfNeeded = false);

    /** Returns true if the cache can load a given file in one way
        or the other */
    bool canLoad (const File& sampleFile) const;

    /** Load an audio file into an AudioSampleBuffer
        @note this loads the entire file into memory in one shot
        */
    BufferPtr loadAudioFile (const File& sampleFile);
    AudioFormatManager&  getAudioFormats() { return formats; }

    void activate (double samplerate, uint32 buffersize);
    void deacitvate();

    inline void setThreadPriority (int priority)
    {
        threadPriority = priority;
        for (TimeSliceThread* t : threads)
            if (t) t->setPriority (threadPriority);
    }

    /** Creates a new layer source from an XML element representation
        of a Layer */
    LayerData* createLayerSource (const XmlElement& layer);

    AudioFormatReader* createReaderFor (const XmlElement& item);
    AudioFormatReader* createReaderFor (const File& file);

   #if defined (HAVE_LVTK)
    LayerData* getLayerData (const URIs& uris, const lvtk::AtomObject& layer, bool realtime);
   #endif

private:
    LayerData* findLayerData (const int32 id) const;
    TimeSliceThread* nextThread();
    HashMap<int32, BufferPtr>   audioBuffers;
    Array<int32> blacklist;

    // HashMap<int32, Shared<MidiMessageSequence> > midiSequences;

    AudioFormatManager&             formats;
    #if 0
    AudioThumbnailCache             thumbs;
    #endif
    OwnedArray<TimeSliceThread>     threads;
    OwnedArray<LayerData>           layers;
    OwnedArray<AudioFormatReader>   readers;
    int threadPriority;
};

#if 0
class AudioPeak :  public AudioThumbnail
{
public:
    AudioPeak (SampleCache& c)
        : AudioThumbnail (128, c.codecs(), c.peaks()),
            data (c) { }

    SampleCache& sampleCache() { return data; }

private:
    SampleCache& data;
};

typedef Shared<AudioPeak> AudioPeakPtr;
#endif

}
