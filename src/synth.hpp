/*
    SamplerSynth.h - This file is part of KSP1
    Copyright (C) 2015  Kushview, LLC. All rights reserved.

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

#include "disposable.hpp"

namespace ksp1 {

class LayerData;
class PatchSet;
class SampleCache;
class SamplerSound;
class URIs;

class SamplerSynth : public juce::Synthesiser,
                     public Disposable {
public:
    class DataLoader {
    public:
        struct Type {
            virtual ~Type() {}
            virtual bool canLoad (const juce::File&) = 0;
        };

        DataLoader (SampleCache& c);
        virtual ~DataLoader();
        virtual bool createSounds() = 0;

    protected:
        SamplerSound* createSound (int note, int id = 0);
        LayerData* createLayerData (SamplerSound& sound);
        juce::OwnedArray<SamplerSound> sounds;
        juce::Array<LayerData*> layers;
        friend class SamplerSynth;
        SampleCache& cache;
    };

    /** Create a new Sampler with an external sample cache */
    static SamplerSynth* create (SampleCache& cache);

    /** Create a synth using the internal cache */
    static SamplerSynth* create();

    /** dtor */
    virtual ~SamplerSynth();

    void clearAllSounds();

    void setVolume (const double val);
    inline void setGain (const double val) { masterGain.set (val); }
    inline const double getGain() const { return masterGain.get(); }

    /** Get the current MIDI channel this sampler operates on */
    const int getMidiChannel() const { return midiChannel.get(); }

    /** Set the current operating MIDI channel */
    void setMidiChannel (int chan);

    /** Insert some LayerData for a given SamplerSound::id */
    bool insertLayerData (uint32_t soundId, LayerData* data);

    /** Insert a SamplerSound for processing */
    bool insertSound (SamplerSound* sound);

    /** Load a sampler from a file */
    bool loadFile (const juce::File& file);

    /** Load a sampler from JSON data */
    bool loadJSON (const juce::String& jsonString);

    /** Load the sampler from XML create with ValueTree::toXmlString() */
    bool loadValueTreeXml (const juce::XmlElement& xml);

    /** Returns the SampleCache used by this SamplerSynth */
    SampleCache& getSampleCache() const { return cache; }

    float getMasterGain() const { return masterGain.get(); }

    LayerData* getLayerData (const int note, const int index);

    void recycleLayerData (LayerData* data);

    /** Change/move a sound to a new root note number */
    SamplerSound* moveSound (uint32_t objectId, int newNote);

    /** Find a sound by object id */
    SamplerSound* getSoundForObjectId (uint32_t oid) const;

    /** Find layer data by object id */
    LayerData* getLayerDataForObjectId (uint32_t oid) const;

    /** Creates a var object representation that is suitable to use
            as JSON serializing and parsing */
    bool getNestedVariant (juce::var& output);

    /** Triggers a note-on event */
    void noteOn (const int midiChannel, const int midiNoteNumber, const float velocity) override;

#if defined(HAVE_LVTK)
    /** Set a property with an LV2 Patch set wrapper
            This method assumes the subject either is a URID and equals URIS::ksp1_Instrument,
            or an atom_Object with an otype of ksp1_Instrument
         */
    void setProperty (const URIs& uris, const PatchSet& set);
    SamplerSound* getSound (const URIs& uris, const lvtk::AtomObject& key);
    LayerData* getLayerData (const URIs& uris, const lvtk::AtomObject& layer);
#endif

protected:
    SamplerSynth (SampleCache& cache);
    SampleCache& cache;

    /** Get a SamplerSound for a given midi note
            @param note The note to search for
            @param createIt create the sound if it doesn't exists
         */
    SamplerSound* findSound (int note, bool createIt);

    friend class DataLoader;

private:
    juce::Atomic<double> masterGain;
    juce::Atomic<int> midiChannel;
    juce::HashMap<int, ksp1::SamplerSound*> soundMap;
    typedef juce::HashMap<int, ksp1::SamplerSound*>::Iterator SoundIterator;
    juce::HashMap<int, ksp1::LayerData*> layerMap;
    typedef juce::HashMap<int, ksp1::LayerData*>::Iterator LayerIterator;

    bool installLoadedData (DataLoader& loader);
};

} // namespace ksp1
