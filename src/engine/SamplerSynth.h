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

#ifndef KSP1_SAMPLER_SYNTH_H
#define KSP1_SAMPLER_SYNTH_H

#include "modules/juce_audio_basics/juce_audio_basics.h"
#include "Types.h"
#include "engine/Disposable.h"

namespace lvtk {
    class Atom;
    class AtomObject;
}

namespace KSP1 {

    class LayerData;
    class PatchSet;
    class SampleCache;
    class SamplerSound;
    class URIs;

    class SamplerSynth : public Synthesiser,
                         public Disposable
    {
    public:

        class DataLoader
        {
        public:

            struct Type {
                virtual bool canLoad (const File&) =0;
            };

            DataLoader (SampleCache& c);
            virtual ~DataLoader();
            virtual bool createSounds() =0;

        protected:
            SamplerSound* createSound (int note, int id = 0);
            LayerData* createLayerData (SamplerSound& sound);
            OwnedArray<SamplerSound> sounds;
            Array<LayerData*> layers;
            friend class SamplerSynth;
            SampleCache& cache;

        };

        /** Create a new Sampler with an external sample cache */
        static SamplerSynth* create (SampleCache& cache);

        /** Create a synth using the internal cache */
        static SamplerSynth* create ();

        /** dtor */
        virtual ~SamplerSynth();

        void clearAllSounds();

        void setVolume (const double val);
        inline void setGain (const double val) { masterGain.set (val); }
        inline const double& getGain() const { return masterGain.get(); }

        /** Get the current MIDI channel this sampler operates on */
        const int& getMidiChannel() const { return midiChannel.get(); }

        /** Set the current operating MIDI channel */
        void setMidiChannel (int chan);

        bool insertLayerData (uint32 soundId, LayerData* data);
        bool loadFile (const File& file);
        bool loadValueTreeXml (const XmlElement& xml);



        SampleCache& getSampleCache() const { return cache; }

        void dumpSounds();

        float getMasterGain() const { return masterGain.get(); }

        /** Set a property with an LV2 Patch set wrapper
            This method assumes the subject either is a URID and equals URIS::ksp1_Instrument,
            or an atom_Object with an otype of ksp1_Instrument
         */
        void setProperty (const URIs& uris, const PatchSet& set);

        SamplerSound* getSound (const URIs& uris, const lvtk::AtomObject& key);

        LayerData* getLayerData (const int note, const int index);
        LayerData* getLayerData (const URIs& uris, const lvtk::AtomObject& layer);

        void recycleLayerData (LayerData* data);

        bool insertSound (SamplerSound* sound);

        SamplerSound* moveSound (uint32_t objectId, int newNote);
        SamplerSound* getSoundByObjectId (uint32_t oid) const;
        LayerData* getLayerDataByObjectId (uint32_t oid) const;

        bool getNestedVariant (var& output);

        void noteOn (const int midiChannel, const int midiNoteNumber, const float velocity) override;

    protected:
        SamplerSynth (SampleCache& cache);
        SampleCache& cache;

        /** Get a SamplerSound for a given midi note
            @param note The note to search for
            @param createIt create the sound if it doesn't exists
         */
        SamplerSound* getSound (int note, bool createIt = true);
        friend class DataLoader;

    private:
        AtomicDouble masterGain;
        AtomicInt midiChannel;
        HashMap<int, KSP1::SamplerSound*> soundMap;
        typedef HashMap<int, KSP1::SamplerSound*>::Iterator SoundIterator;
        HashMap<int, KSP1::LayerData*> layerMap;
        typedef HashMap<int, KSP1::LayerData*>::Iterator LayerIterator;

        bool installLoadedData (DataLoader& loader);
    };
}

#endif // KSP1_SAMPLER_SYNTH_H
