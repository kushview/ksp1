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

#include <memory>

#include "juce.hpp"
#include "ksp1.hpp"

namespace ksp1 {

class LayerData;

using FormatManager = juce::AudioFormatManager;
using FormatReader  = juce::AudioFormatReader;
using SampleBuffer  = juce::AudioSampleBuffer;
using BufferPtr     = std::shared_ptr<SampleBuffer>;

class SampleCache {
public:
    SampleCache (FormatManager&);
    ~SampleCache();

    LayerData* getLayerData (const bool createIfNeeded = false);

    /** Returns true if the cache can load a given file in one way
        or the other */
    bool canLoad (const juce::File& sampleFile) const;

    /** Load an audio file into an AudioSampleBuffer
        @note this loads the entire file into memory in one shot
     */
    BufferPtr loadAudioFile (const juce::File& sampleFile);

    FormatManager& getAudioFormats() { return formats; }

    void activate (double samplerate, int buffersize);
    void deacitvate();

    inline void setThreadPriority (int priority) {
        threadPriority = priority;
        // for (auto* const thread : threads)
        //     if (thread)
        //         thread->setPriority (juce::Thread::Priority::normal);
    }

    /** Creates a new layer source from an XML element representation
        of a Layer */
    LayerData* createLayerSource (const juce::XmlElement& layer);
    FormatReader* createReaderFor (const juce::XmlElement& item);
    FormatReader* createReaderFor (const juce::File& file);

#if defined(HAVE_LVTK)
    LayerData* getLayerData (const URIs& uris, const lvtk::AtomObject& layer, bool realtime);
#endif

private:
    LayerData* findLayerData (const int id) const;
    juce::HashMap<int, BufferPtr> audioBuffers;
    juce::Array<int> blacklist;

    FormatManager& formats;
    juce::ReferenceCountedArray<LayerData> layers;
    juce::OwnedArray<FormatReader> readers;
    int threadPriority { 3 };
};

} // namespace ksp1
