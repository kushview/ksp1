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
#include "MediaObject.h"
#include "InstrumentLoader.h"
#include "engine/SamplerKeys.h"

namespace KSP1 {

static inline Identifier fxSendIdentifier (int sendNumber)
{
    static const Identifier fxSendSlug      = "fxSend";
    String s (fxSendSlug.toString());
    s << String("-") << String (sendNumber);
    return Identifier (s);
}

class SamplerSynth;

class LayerItem :  public kv::ObjectModel
{
public:
    LayerItem();
    LayerItem (const ValueTree& data);
    ~LayerItem();

    inline bool isEmpty() const { return ! objectData.hasProperty (Tags::file); }
    inline bool isValid() const { return objectData.isValid() && objectData.hasType (Tags::layer)
                                    && (int32) objectData.getProperty(Tags::index, -1) >= 0
                                    && (int32) objectData.getProperty (Tags::note, -1) >= 0; }
    int32 index() const;
    int32 getNote() const;
    
    double getStart() const     { return objectData.getProperty ("start"); }
    double getLength() const    { return objectData.getProperty ("length"); }
    
    inline String fileString() const { return get (Tags::file); }

    inline void setFile (const File& file)
    {
        set (Tags::file, file.getFullPathName());
        set (Tags::name, file.getFileNameWithoutExtension());
    }

    inline void setStartTime (double time)  { set (Tags::start,  time); }
    inline void setEndTime (double time)    { set (Tags::length, time - startTime()); }
    inline double startTime() const { return (double) get (Tags::start); }
    inline double endTime()   const { return startTime() + (double)get(Tags::length); }

    inline double gain() const
    {
        double vol = get (Tags::volume);
        return Decibels::decibelsToGain (vol);
    }

    inline double panning()   const { return get (Tags::panning); }
    inline double pitch()     const { return get (Tags::pitch); }
    inline double cutoff()    const { return get (Tags::cutoff);  }
    inline double resonance() const { return get (Tags::resonance); }

    inline void getVelocityRange (Range<double>& vel) const
    {
        vel.setStart (get (Tags::velocityLower));
        vel.setEnd (get (Tags::velocityUpper));
    }

    inline void setRange (const Range<double>& range)
    {
        setEndTime (range.getEnd());
        setStartTime (range.getStart());
    }

    inline void getRange (Range<double>& range) const
    {
        range.setStart (startTime());
        range.setEnd (endTime());
    }

    inline double duration() const
    {
        Range<double> r;
        getRange (r);
        return r.getLength();
    }

    inline ValueTree getParentValueTree() const {
        return objectData.getParent();
    }

    inline const var&
    getProperty (const Identifier& prop) const {
        return get (prop);
    }

    inline void
    setProperty (const Identifier& prop, const var& val)
    {
        set (prop, val);
    }

    inline LayerItem&
    operator= (const LayerItem& other)
    {
        objectData = other.node();
        return *this;
    }

#if 0
    inline ForgeRef writeAtomObject (Forge& forge) const
    {
        const ValueTree parent = getParentValueTree();
        const uint32 oid = (uint32)(int) getProperty (Tags::id);
        const uint32 pid = (uint32)(int) parent.getProperty (Tags::id, 0);
        PropertyWriter writer (forge, objectData, forge.uris.ksp1_Layer, oid, pid);
        return writer.writeAtomObject();
    }

    inline ForgeRef writeAtomSubject (Forge& forge) const
    {
        const ValueTree parent = getParentValueTree();
        const uint32 oid = (uint32)(int) getProperty (Tags::id);
        const uint32 pid = (uint32)(int) parent.getProperty (Tags::id, 0);
        PropertyWriter writer (forge, objectData, forge.uris.ksp1_Layer, oid, pid);
        return writer.writeAtomObjectIncluding ({ Tags::parent, Tags::index });
    }
#endif

private:
    const var& get (const Identifier& prop) const;
    void set (const Identifier& prop, const var& val);
    void setMissingProperties();
    void fixPropertyTypes();

    friend class KeyItem;
    friend class Instrument;
};

/** The model representation of a key on the sampler */
class KeyItem :  public kv::ObjectModel
{
public:
    explicit KeyItem (bool setMissing = true);
    KeyItem (const int note, const bool setMissing = true);
    KeyItem (const ValueTree& k);

    bool isValid()      const { return objectData.isValid(); }
    bool isNotValid()   const { return ! objectData.isValid(); }

    LayerItem addLayer (const File& file);
    LayerItem addLayer (const LayerItem& layer);
    LayerItem getLayer (int index) const;
    void removeLayer (int index);

    void clearLayers();
    int countLayers() const;

    double getVolume()     const { return (double) objectData.getProperty (Tags::volume); }
    Value getVolumeValue()       { return objectData.getPropertyAsValue (Tags::volume, nullptr); }

    int32 getTriggerMode() const { return (int32) objectData.getProperty (Tags::triggerMode); }
    Value getTriggerModeValue()  { return objectData.getPropertyAsValue (Tags::triggerMode, nullptr); }
    void setTriggerMode (const int t) { objectData.setProperty (Tags::triggerMode, t, 0); }

    int32 getVoiceGroup()  const { return (int32) objectData.getProperty (Tags::voiceGroup); }
    Value getVoiceGroupValue()   { return objectData.getPropertyAsValue (Tags::voiceGroup, nullptr); }

    int32 getNote()        const { return (int32) objectData.getProperty (Tags::note, -1); }
    Value getNoteValue()         { return objectData.getPropertyAsValue (Tags::note, nullptr); }

    KeyItem& operator= (const KeyItem& other);

private:
    int32 nlayers;

    void set (const Identifier& k, const var& v);
    void setAdsr (float a, float d, float s, float r);
    void setMissingProperties();

    friend class Instrument;
};

class Instrument : public MediaObject
{
public:
    Instrument (const String& name);
    Instrument (const ValueTree& data);
    Instrument (const Instrument& other);

    ~Instrument();

    void setName (const String& name);
    String getName() const;

    void clear();

    void getSamples (StringArray& samples) const;

    int getNumSounds() const { return getNumChildren(); }
    KeyItem getSound (int index) const;
    void setActiveSoundIndex (int index);
    KeyItem getActiveSound() const;
    
    KeyItem addKey (int32 noteNumber, int32 keySpan = 0);
    KeyItem getKey (int32 noteNumber, bool setMissing = false) const;
    KeyItem getOrCreateKey (int32 noteNumber);

    LayerItem getLayer (int32 key, int32 layer);

    XmlElement* createXml();

    void loadFromXml (const XmlElement& node);
    void load (const File& file, ProgressSink& sink);

    inline float getVolume() const { return (float) node().getProperty (Tags::volume, 0.0f); }
    inline void setVolume (float vol) { node().setProperty (Tags::volume, vol, nullptr); }
    void setMissingProperties (bool recursive = true);

    Instrument& operator= (const Instrument& other);

    inline void selectNote (int note) {  selectedNote = note; }
    inline const int currentKeyId() const { return selectedNote; }

    inline void setFile (const File& f) { file = f; }
    inline File getFile() const { return file; }

    void print()
    {
        auto copy = objectData.createCopy();
        DBG (copy.toXmlString());
    }

    void sortKeys();

private:
    File file;
    friend class Pattern;
    int selectedNote;

};

using InstrumentPtr = ReferenceCountedObjectPtr<Instrument>;

}
