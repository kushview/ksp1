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

#include "engine/SamplerKeys.h"

namespace KSP1 {

class LowPassFilter;
class PatchSet;
class SampleCache;
class SamplerSound;
class SamplerSynth;
class SamplerVoice;
struct URIs;

class LayerData
{
public:
    LayerData (SampleCache&, int layerId);
    ~LayerData();

    /** Create a DynamicObject representing this LayerData */
    DynamicObject::Ptr createDynamicObject() const;

    void setStartTime (double timeIn);
    void setEndTime (double timeOut);

    /** Set the volume of this Layer */
    void setVolume (const double vol);

    /** Returns the audio file this data was loaded from if any */
    const File& getAudioFile() const { return currentFile; }

    /** Get the frame offset */
    const int64& getOffset() const { return offset; }

    /** Get the length frames */
    const int64& getLength() const { return length; }

    /** Get the start frame in the parent sound */
    const int64& getStart() const { return start; }

    /** Get the parent SamplerSound's ID for this LayerData */
    const uint32& getParent() const { return parent; }

    /** Get the SamplerSound this layer belongs to.  Returns
        nullptr if it isn't a part of a sound */
    SamplerSound* getSound() const { return sound; }

    /** Load an audio file into memory and set the render buffer */
    bool loadAudioFile (const File& audioFile);

    /** Reset properties to default values */
    void reset();

    /** Restore properties from a JSON formatted var */
    void restoreFromJSON (const var& json);

    /** Restore properties from an XmlElement */
    void restoreFromXml (const XmlElement& e);

   #if defined (HAVE_LVTK)
    /** Set a property using a PatchSet */
    void setProperty (const URIs& uris, const PatchSet& set);

    /** Set a single property with an LV2 Atom */
    void setProperty (const URIs& uris, uint32_t prop, const lvtk::Atom&, bool realtime = true);

    /** Set all properties contained within the given atom object */
    void setAtomObject (const URIs& uris, const lvtk::AtomObject& object, bool realtime = true);

    /** Write Layer properties to an AtomObject */
    ForgeRef writeAtomObject (Forge& forge);
   #endif

protected:
    Atomic<double> gain;
    Atomic<double> pitch;
    Atomic<double> panning;
    Atomic<double> cutoff;
    Atomic<double> resonance;
    Atomic<int>  in, out;
    Range<double> velocityRange;
    int64 start, offset, length;

    /** Set the type (URID) of this Layer (unused) */
    void setType (const uint32 t) { type = t; }

private:
    friend class SampleCache;
    friend class SamplerSound;
    friend class SamplerSynth;
    friend class SamplerVoice;

    SampleCache&    cache;
    uint32          type;
    int64           lengthInSamples;
    uint32          numChannels;
    double          sampleRate;

    const int32   id;
    uint32        parent;
    SamplerSound* sound;
    int           index;
    int           note;
    File          currentFile;

    std::shared_ptr<AudioSampleBuffer> scratch;
    AudioSampleBuffer* renderBuffer = nullptr;

    ScopedPointer<LowPassFilter> filter;

    void startNote (int voice, const KeyInfo& key);
    const float* getSampleData (int32 chan, int32 frame) const;
};

}
