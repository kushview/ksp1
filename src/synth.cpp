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

#include "juce.hpp"

#include "cache.hpp"
#include "layerdata.hpp"
#include "samplerkeys.hpp"
#include "sounds.hpp"
#include "synth.hpp"
#include "voice.hpp"

namespace ksp1 {

using Decibels    = juce::Decibels;
using File        = juce::File;
using JSON        = juce::JSON;
using String      = juce::String;
using var         = juce::var;
using XmlDocument = juce::XmlDocument;
using XmlElement  = juce::XmlElement;

static std::unique_ptr<juce::AudioFormatManager> sAudioFormats;
static std::unique_ptr<SampleCache> sSampleCache;

static juce::Array<SamplerSynth*> sCacheUsers;

struct ZoneSorter {
    static int compareElements (const SamplerSound* first, const SamplerSound* second) {
        if (first->getRootNote() < second->getRootNote())
            return -1;
        else if (first->getRootNote() == second->getRootNote())
            return 0;
        else if (second->getRootNote() < first->getRootNote())
            return 1;

        jassertfalse;
        return 0;
    }
};

class JSONSynthLoader : public SamplerSynth::DataLoader {
public:
    JSONSynthLoader (const var& json, SampleCache& c)
        : SamplerSynth::DataLoader (c), data (json) {}

    bool createSounds() override {
        //            for (int i = 0; i < data.size(); ++i)
        {
            const var sounds = data.getProperty ("sounds", var());

            for (int si = 0; si < sounds.size(); ++si) {
                const var sound      = sounds[si];
                SamplerSound* ssound = createSound (sound.getProperty ("note", -1),
                                                    sound.getProperty ("id", 0));
                if (! ssound)
                    continue;

                ssound->restoreFromJSON (sound);

                const var layers = sound.getProperty ("layers", var());
                for (int li = 0; li < layers.size(); ++li) {
                    const var layer (layers[li]);
                    if (LayerData* ld = createLayerData (*ssound))
                        ld->restoreFromJSON (layer);
                }

                ssound->setDefaultLength();
            }
        }

        return true;
    }

private:
    const var& data;
};

class ValueTreeXmlSynthLoader : public SamplerSynth::DataLoader {
public:
    ValueTreeXmlSynthLoader (const XmlElement& e, SampleCache& c)
        : SamplerSynth::DataLoader (c), data (e) {}

    ~ValueTreeXmlSynthLoader() {}

    bool createSounds() override {
        bool res = true;

        for (int i = data.getNumChildElements(); --i >= 0;) {
            const XmlElement* c = data.getChildElement (i);
            if (c->hasTagName ("key"))
                res = createKey (*c);

            if (! res) {
                DBG ("Error creating Key");
                break;
            }
        }

        return res;
    }

protected:
    const XmlElement& data;
    bool createKey (const XmlElement& key) {
        bool res = false;

        if (SamplerSound* sound = createSound (key.getIntAttribute ("note", -1))) {
            sound->setVolume (key.getDoubleAttribute ("volume", 0));
            sound->setTriggerMode (key.getIntAttribute ("triggerMode", TriggerMode::Retrigger));
            sound->setVoiceGroup (key.getIntAttribute ("voiceGroup", -1));
            sound->setKeyLength (key.getIntAttribute ("length", 0));

            for (int i = 0; i < key.getNumChildElements(); ++i) {
                XmlElement* c = key.getChildElement (i);
                if (c->hasTagName ("layer"))
                    res = createLayer (*sound, *c);
            }

            sound->setDefaultLength();
            res = true;
        }

        return res;
    }

    bool createLayer (SamplerSound& sound, const XmlElement& layer) {
        if (LayerData* data = DataLoader::createLayerData (sound)) {
            data->restoreFromXml (layer);
            return true;
        }

        DBG ("Error creating LayerSource Object");
        return false;
    }
};

SamplerSynth::DataLoader::DataLoader (SampleCache& c) : cache (c) {}
SamplerSynth::DataLoader::~DataLoader() {
    for (auto* s : sounds)
        s->layerSources().clearQuick();
    sounds.clear();
    layers.clear();
}

SamplerSound* SamplerSynth::DataLoader::createSound (int note, int id) {
    if (SamplerSound* s = new SamplerSound (note, id))
        return sounds.add (s);
    return nullptr;
}

LayerData* SamplerSynth::DataLoader::createLayerData (SamplerSound& sound) {
    if (LayerData* source = cache.getLayerData (true)) {
        layers.add (source);
        source->note = sound.getRootNote();
        jassert (sounds.contains (&sound));
        sound.insertLayerData (source);
        return source;
    }

    return nullptr;
}

//=============================================================================
SamplerSynth::SamplerSynth (SampleCache& c)
    : cache (c), masterGain (1.0f), soundMap (256), layerMap (256) {
    midiChannel.set (0);

    for (int i = 0; i < 32; ++i)
        addVoice (new SamplerVoice (*this, i + 1));
}

SamplerSynth::~SamplerSynth() {
    sCacheUsers.removeFirstMatchingValue (this);

    if (sCacheUsers.size() == 0) {
        sSampleCache.reset();
        sAudioFormats.reset();
    }
}

void SamplerSynth::clearAllSounds() {
    allNotesOff (0, false);
    for (int note = 0; note < 128; ++note)
        if (auto* sound = findSound (note, false))
            sound->clearSources();
}

SamplerSynth* SamplerSynth::create (SampleCache& c) {
    try {
        SamplerSynth* synth = new SamplerSynth (c);
        sCacheUsers.add (synth);
        return synth;
    } catch (...) {
        return nullptr;
    }
}

SamplerSynth* SamplerSynth::create() {
    if (! sSampleCache) {
        sAudioFormats.reset (new juce::AudioFormatManager());
        sAudioFormats->registerBasicFormats();
        sSampleCache.reset (new SampleCache (*sAudioFormats));
    }

    return SamplerSynth::create (*sSampleCache);
}

bool SamplerSynth::insertLayerData (uint32_t soundId, LayerData* data) {
    jassert (data);
    if (SamplerSound* sound = getSoundForObjectId (soundId)) {
        if (sound->insertLayerData (data)) {
            layerMap.set (data->id, data);
            return true;
        }
    }

    return false;
}

SamplerSound* SamplerSynth::findSound (int note, bool createIt) {
    return nullptr;
}

void SamplerSynth::setVolume (const double val) {
    const double gain (Decibels::decibelsToGain (val));
    masterGain.set (gain);
}

void SamplerSynth::setMidiChannel (int chan) {
    midiChannel.set (chan);
    SoundIterator iter (soundMap);

    while (iter.next())
        if (iter.getValue())
            iter.getValue()->setMidiChannel (chan);
}

bool SamplerSynth::loadFile (const File& file) {
    if (file.hasFileExtension (".xml")) {
        if (auto xml = std::unique_ptr<XmlElement> (XmlDocument::parse (file)))
            return loadValueTreeXml (*xml);
    } else if (file.hasFileExtension (".json")) {
        var json = JSON::parse (file);
        JSONSynthLoader loader (json, getSampleCache());
        if (loader.createSounds())
            return installLoadedData (loader);
    }

    return false;
}

bool SamplerSynth::loadJSON (const String& jsonString) {
    const var json (JSON::fromString (jsonString));
    JSONSynthLoader loader (json, getSampleCache());
    if (loader.createSounds())
        return installLoadedData (loader);
    return false;
}

bool SamplerSynth::loadValueTreeXml (const XmlElement& e) {
    bool res = false;

    if (e.hasTagName ("instrument") || e.hasTagName ("instruments")) {
        ValueTreeXmlSynthLoader loader (e, getSampleCache());
        if (loader.createSounds()) {
            res = installLoadedData (loader);
            if (! res) {
                DBG ("Could not install new data");
            }
        } else {
            DBG ("Could not create Sounds and Layers from XML data");
        }
    }

    return res;
}

bool SamplerSynth::installLoadedData (DataLoader& loader) {
    layerMap.clear();
    soundMap.clear();
    Synthesiser::clearSounds(); //!!! deletes sounds that aren't referenced elsewhere.

    for (SamplerSound* sound : loader.sounds) {
        addSound (sound);
        soundMap.set (sound->id, sound);

        for (LayerData* layer : *sound) {
            layerMap.set (layer->id, layer);
        }

        sound->setDefaultLength();
    }

    const bool loaded = (layerMap.size() == loader.layers.size() && soundMap.size() == loader.sounds.size());

    loader.sounds.clearQuick (false);
    loader.layers.clearQuick();

    return loaded;
}

void SamplerSynth::recycleLayerData (LayerData* data) {
    jassert (data);
    layerMap.removeValue (data);
    data->reset();
}

bool SamplerSynth::insertSound (SamplerSound* sound) {
    jassert (! soundMap.contains (sound->id));
    soundMap.set (sound->id, sound);
    addSound (sound);
    sound->setMidiChannel (midiChannel.get());
    return true;
}

SamplerSound* SamplerSynth::getSoundForObjectId (uint32_t oid) const {
    return (oid != 0) ? soundMap[static_cast<int> (oid)] : nullptr;
}

void SamplerSynth::noteOn (const int channel, const int note, const float velocity) {
#if 1
    Synthesiser::noteOn (channel, note, velocity);
#else

    const int numSounds = zones.size();
    for (int i = 0; i < numSounds; ++i) {
        SynthesiserSound* const sound = zones.getUnchecked (i);

        if (sound->appliesToNote (note)
            && sound->appliesToChannel (channel)) {
            // If hitting a note that's still ringing, stop it first (it could be
            // still playing because of the sustain or sostenuto pedal).
            for (int j = voices.size(); --j >= 0;) {
                SynthesiserVoice* const voice = voices.getUnchecked (j);

                if (voice->getCurrentlyPlayingNote() == note
                    && voice->isPlayingChannel (channel))
                    stopVoice (voice, 1.0f, true);
            }

            startVoice (findFreeVoice (sound, channel, note, shouldStealNotes),
                        sound,
                        channel,
                        note,
                        velocity);
        }
    }
#endif
}

SamplerSound* SamplerSynth::moveSound (uint32_t oid, int newNote) {
    if (SamplerSound* sound = getSoundForObjectId (oid)) {
        sound->setRootNote (newNote);
        return sound;
    }

    return nullptr;
}

LayerData* SamplerSynth::getLayerData (const int note, const int index) {
    jassertfalse; // remove this function;
    return nullptr;
}

LayerData* SamplerSynth::getLayerDataForObjectId (uint32_t oid) const {
    return (oid != 0) ? layerMap[static_cast<int> (oid)]
                      : nullptr;
}

bool SamplerSynth::getNestedVariant (var& json) {
#if 0
    DynamicObject::Ptr synth = new DynamicObject();
    synth->setProperty (Slugs::id, (int) 0);
    synth->setProperty (Slugs::volume, Decibels::gainToDecibels ((double) getMasterGain()));
    var sound;
    SoundIterator iter (soundMap);
    while (iter.next())
    {
        if (SamplerSound* s = iter.getValue())
            if (DynamicObject::Ptr d = s->createDynamicObject())
                sound.append (d.get());
    }

    if (sound.size() > 0)
        synth->setProperty ("sounds", sound);

    synth->write
    json = synth->gerVar;

    return true;
#endif
    jassertfalse; // FIXME:
    return false;
}

#if defined(HAVE_LVTK)

void SamplerSynth::setProperty (const URIs& uris, const PatchSet& set) {
    if (uris.slugs_volume == set.property.as_urid()) {
        setVolume (set.value.as_double());
    }
}

SamplerSound* SamplerSynth::getSound (const URIs& uris, const lvtk::AtomObject& key) {
    if (SamplerSound* s = getSoundForObjectId (key.id()))
        return s;

    const lvtk::Atom note;
    lv2_atom_object_get (key, uris.slugs_note, &note, 0);
    if (! note) {
        return nullptr;
    }

    const int noteNumber = note.as_int();
    for (int i = getNumSounds(); --i >= 0;) {
        SynthesiserSound* s = Synthesiser::getSound (i);
        if (s->appliesToNote (noteNumber))
            return dynamic_cast<SamplerSound*> (s);
    }

    return nullptr;
}

LayerData* SamplerSynth::getLayerData (const URIs& uris, const lvtk::AtomObject& obj) {
    if (LayerData* layer = getLayerDataForObjectId (obj.id()))
        return layer;

    const lvtk::Atom index, parent;
    lv2_atom_object_get (obj,
                         uris.slugs_index,
                         &index,
                         uris.slugs_parent,
                         &parent,
                         0);

    if (parent && index) {
        if (SamplerSound* sound = getSoundForObjectId (parent.as_urid()))
            if (LayerData* layer = sound->getLayer (index.as_int()))
                return layer;
    }

    if (! parent) {
        DBG ("Layer has no parent");
    }

    if (! index) {
        DBG ("Layer has no index");
    }

    return nullptr;
}
#endif

} // namespace ksp1
