/*
    This file is part of KSP1
    Copyright (C) 2014-2019  Kushview, LLC. All rights reserved.

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

#include "./engine/SamplerKeys.h"

namespace KSP1 {

class SamplerSynth;
class LayerData;
class KeyItem;

/** A base for all sounds in the sampler */
class SoundBase :  public SynthesiserSound
{
public:
    SoundBase() { }
    virtual ~SoundBase() { }
};


/** The default sampler sound, maintains a group of layer data */
class SamplerSound : public SoundBase,
                     private Value::Listener
{
public:
    typedef Array<LayerData*> LayerSources;

    class Lock
    {
    public:
        Lock (SamplerSound& snd) : sound (snd) { sound.dataLock.lock(); }
        ~Lock() { sound.dataLock.unlock(); }
    
    private:
        SamplerSound& sound;
    };

    SamplerSound (int noteNumber, int soundID = 0);
    ~SamplerSound();

    bool appliesToNote (const int note) override;
    bool appliesToChannel (const int chan) override;

    const KeyInfo& getKeyInfo() const { return key; }
    int getRootNote() const;
    void setRootNote (int n);

    int32 getNumLayers() const { return activeLayers.size(); }
    const int64& getStart() const { return start.get(); }

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
    inline void setLength (int64 nframes)
    {
        int64 longest = longestLayerFrames();
        if (nframes > longest || nframes <= 0)
            nframes = longest;

        duration.set (nframes);
    }

    void restoreFromJSON (const var& json);

    /** Set the sounds default length. This is the longest of all layers */
    void setDefaultLength();

    /** Get the total length of this sound.  This is thread/realtime safe */
    int64 length() const { return duration.get(); }

    /** Set an fx bus volume
        @param busId The FX bus to ChangeBroadcaster
        @param val The value to use (0.0 - 1.0f)
     */
    inline void setFxSendValue (int busId, float val)
    {
        if (! isPositiveAndBelow (busId, 4))
            return;

        const float gain = Decibels::decibelsToGain ((val * 66.0f) - 60.0f);

        Lock lock (*this);
        key.fxGain [busId] = gain;
    }

    /** Set the key length */
    inline void setKeyLength (const int length)
    {
        Lock lock (*this);
        key.length = length >= 0 ? length : 0;
    }

    /** Set the master gain for this sound */
    inline void setGain (float gain)
    {
        const float volume = Decibels::gainToDecibels (gain);

        Lock lock (*this);
        key.volume = volume;
        key.gain   = gain;
    }

    /** Set this sounds trigger mode */
    inline void setTriggerMode (TriggerMode trigger)
    {
        Lock lock (*this);
        key.triggerMode = trigger;
    }

    /** Assign this sounds voice group */
    inline void setVoiceGroup (int group)
    {
        Lock lock (*this);
        key.voiceGroup = group;
    }

    /** Set the random pitch factor for this sound */
    inline void setRandomPitchFactor (float f)
    {
        Lock lock (*this);
        key.randomPitch = f;
    }

    inline void setPitch (float pitch)
    {
        Lock lock (*this);
        key.pitch = pitch;
    }

    inline void setPan (float pan)
    {
        Lock lock (*this);
        // noop
    }

    /** Set the master volume (in decibels) for this sound */
    inline void setVolume (const double vol)
    {
        const double gain = Decibels::decibelsToGain (vol);

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

    float getPitchOffsetForNote (const int note) const { return (float) note - key.note; }

    /** Add a new source to be processed */
    bool insertLayerData (LayerData* getLayer);

    /** Remove a source from processing */
    void removeLayer (LayerData* getLayer);

    /** Create a dynamic object from this sound */
    DynamicObject::Ptr createDynamicObject() const;

   #if defined (HAVE_LVTK)
    /** writes an atom object */
    ForgeRef writeAtomObject (Forge& forge);
    /** Set a property from a PatchSet message */
    void setProperty (const URIs& uris, const PatchSet& set);
   #endif

    const int getObjectId() const { return id; }

    void bindTo (const KeyItem&);

private:
    const int id;
    KeyInfo key;

    AtomicFrame     start, duration;
    DataLock        dataLock;
    LayerSources    activeLayers;

    BigInteger midiChans;
    BigInteger midiNotes;

    Value noteValue, lengthValue, volumeValue,
          panningValue, pitchValue;

    /** Returns the longest frame count among all layers */
    int64 longestLayerFrames() const;
    
    void valueChanged (Value&) override;

    friend class Lock;
    friend class SamplerSynth;
};

using SamplerSoundPtr = ReferenceCountedObjectPtr<SamplerSound>;

}
