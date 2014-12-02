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

#ifndef KSP1_SAMPLER_BASE_H
#define KSP1_SAMPLER_BASE_H

#include "engine/SamplerKeys.h"
#include "Forge.h"
#include "Types.h"

namespace KSP1 {

    class SamplerSynth;
    class LayerData;

    /** A base for all sounds in the sampler */
    class SoundBase :  public SynthesiserSound
    {
    public:
        SoundBase() { }
        virtual ~SoundBase() { }
    };


    /** The default sampler sound, maintains a group of 'layers' e.g. samples */
    class SamplerSound : public SoundBase
    {
    public:

        typedef Array<LayerData*> LayerSources;

        SamplerSound (int keyid, int soundID);
        virtual ~SamplerSound() { }

        bool appliesToNote (const int note);
        bool appliesToChannel (const int chan);

        const KeyInfo& getKey() const { return key; }
        int getRootNote() const;
        void setRootNote (int n);

        int32 numLayers() const { return activeLayers.size(); }

        LayerData* getLayer (int32 index) const { return activeLayers.getUnchecked (index); }
        LayerData** begin() const { return activeLayers.begin(); }
        LayerData** end()   const { return activeLayers.end(); }

        /** MIDI channel assignment.  Set the channel this sound responds to */
        void setMidiChannel (int chan);

        /** Get an array of all the active layers on this sound */
        LayerSources& layerSources() { return activeLayers; }

        /** Acquire the data lock
            Voices MUST call this when reading/writing data from this sound */
        inline bool acquire() { return dataLock.acquire(); }

        /** Release the data lock
            Voices MUST call this when reading/writing data from this sound
            is finished */
        inline void release() { dataLock.release(); }

        /** Specify a length in frames for this sound.
            Currently this shows the same behaviour as setDefaultLength() */
        inline
        void setLength (int64 nframes)
        {
            int64 longest = longestLayerFrames();
            if (nframes > longest || nframes <= 0)
                nframes = longest;

            while (! duration.set (nframes));
        }

        void setProperty (const URIs& uris, const PatchSet& set);

        /** Set the sounds default length. This is the longest of all layers */
        inline void setDefaultLength() {
            setLength (0);
        }

        /** Get the total length of this sound.  This is thread/realtime safe */
        int64 length() const { return duration.get(); }


        /** Set an fx bus volume
            @param busId The FX bus to ChangeBroadcaster
            @param val The value to use (0.0 - 1.0f)
        */
        inline void
        setFxSendValue (int busId, float val)
        {
            if (! isPositiveAndBelow (busId, 4))
                return;

            const float gain = Decibels::decibelsToGain ((val * 66.0f) - 60.0f);

            Lock lock (*this);
            key.fxGain [busId] = gain;
        }

        /** Set the key length */
        inline void setKeyLength (const int len) {
            Lock lock (*this);
            key.length = len >= 0 ? len : 0;
        }

        /** Set the master gain for this sound */
        inline void
        setGain (float gain)
        {
            const float volume = Decibels::gainToDecibels (gain);

            Lock lock (*this);
            key.volume = volume;
            key.gain   = gain;
        }

        /** Set this sounds trigger mode */
        inline void
        setTriggerMode (TriggerMode trigger)
        {
            Lock lock (*this);
            key.triggerMode = trigger;
        }

        /** Assign this sounds voice group */
        inline void
        setVoiceGroup (int group)
        {
            Lock lock (*this);
            key.voiceGroup = group;
        }

        /** Set the random pitch factor for this sound */
        inline void
        setRandomPitchFactor (float f)
        {
            Lock lock (*this);
            key.randomPitch = f;
        }

        /** Set the master volume (in decibels) for this sound */
        inline void
        setVolume (float vol)
        {
            float gain = Decibels::decibelsToGain (vol);

            Lock lock (*this);
            key.volume = vol;
            key.gain = gain;
        }

        /** Set the attack time of this sound in seconds */
        void setAttack (double ratio);

        /** Set the decay time of this sound in seconds */
        void setDecay (double ratio);

        /** Set the sustain level of this sound */
        void setSustain (double level);

        /** Set the release time of this sound */
        void setRelease (double ratio);

        /** Clear all sources (layers) for this sound */
        void clearSources();

        class Lock {
        public:
            Lock (SamplerSound& snd) : sound (snd) { sound.dataLock.lock(); }
            ~Lock() { sound.dataLock.unlock(); }
        private:
            SamplerSound& sound;
        };

        float getPitchOffsetForNote (const int note) {
            return (float) note - key.note;
        }

        /** Add a new source to be processed */
        bool insertLayerData (LayerData* getLayer);

        /** Remove a source from processing */
        void removeLayer (LayerData* getLayer);

        /** create a dynamic object from this sound */
        DynamicObject::Ptr createDynamicObject() const;

        /** writes an atom object */
        ForgeRef writeAtomObject (Forge& forge);

    private:

        const int32 id;
        KeyInfo key;

        AtomicFrame     start, duration;
        DataLock        dataLock;
        LayerSources    activeLayers;

        BigInteger midiChans;
        BigInteger midiNotes;


        /** @internal Returns the longest frame count among all layers */
        int64 longestLayerFrames() const;

        friend class Lock;
        friend class SamplerSynth;

    };
}

#endif   // KSP1_SAMPLER_BASE_H
