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

#include "juce.hpp"

#include "samplerkeys.hpp"

namespace ksp1 {

class LayerItem;
class LowPassFilter;
class SampleCache;
class SamplerSound;
class SamplerSynth;
class SamplerVoice;

class LayerData : public juce::ReferenceCountedObject {
public:
    LayerData (SampleCache&, int layerId);
    ~LayerData();

    /** Create a DynamicObject representing this LayerData */
    juce::DynamicObject::Ptr createDynamicObject() const;

    void setStartTime (double timeIn);
    void setEndTime (double timeOut);

    /** Set the volume of this Layer */
    void setVolume (const double vol);

    /** Returns the audio file this data was loaded from if any */
    const juce::File& getAudioFile() const { return currentFile; }

    /** Get the frame offset */
    constexpr int64_t getOffset() const noexcept { return offset; }

    /** Get the length frames */
    constexpr int64_t getLength() const noexcept { return length; }

    /** Get the start frame in the parent sound */
    constexpr int64_t getStart() const noexcept { return start; }

    /** Get the parent SamplerSound's ID for this LayerData */
    constexpr uint32_t getParent() const noexcept { return parent; }

    /** Get the SamplerSound this layer belongs to.  Returns
        nullptr if it isn't a part of a sound */
    constexpr SamplerSound* getSound() const noexcept { return sound; }

    /** Load an audio file into memory and set the render buffer */
    bool loadAudioFile (const juce::File& audioFile);

    /** Reset properties to default values */
    void reset();

    void bindTo (const LayerItem&);

    /** Restore properties from a JSON formatted var */
    void restoreFromJSON (const juce::var& json);

    /** Restore properties from an XmlElement */
    void restoreFromXml (const juce::XmlElement& e);

#if defined(HAVE_LVTK)
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
    juce::Atomic<double> gain;
    juce::Atomic<double> pitch;
    juce::Atomic<double> panning;
    juce::Atomic<double> cutoff;
    juce::Atomic<double> resonance;
    juce::Atomic<int> in, out;
    juce::Range<double> velocityRange;
    int64_t start, offset, length;

    /** Set the type (URID) of this Layer (unused) */
    void setType (const uint32_t t) { type = t; }

private:
    friend class SampleCache;
    friend class SamplerSound;
    friend class SamplerSynth;
    friend class SamplerVoice;

    SampleCache& cache;
    uint32_t type;
    int64_t lengthInSamples;
    uint32_t numChannels;
    double sampleRate;

    const int id;
    uint32_t parent;
    SamplerSound* sound;
    int index;
    int note;
    juce::File currentFile;

    std::shared_ptr<juce::AudioSampleBuffer> scratch;
    juce::AudioSampleBuffer* renderBuffer = nullptr;

    std::unique_ptr<LowPassFilter> filter;

    void startNote (int voice, const KeyInfo& key);
    const float* getSampleData (int chan, int frame) const;
    void restoreFromJSON (const juce::File& kitDir, const juce::var& json);
};

using LayerPtr = juce::ReferenceCountedObjectPtr<LayerData>;

} // namespace ksp1
