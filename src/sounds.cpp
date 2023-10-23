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

#include "ksp1.hpp"

#include "layerdata.hpp"
#include "sounds.hpp"
#include "synth.hpp"
#include "util.hpp"

namespace ksp1 {
using DynamicObject = juce::DynamicObject;
using Decibels      = juce::Decibels;

static int lastSoundIdSeed = 0;

SamplerSound::SamplerSound (int noteNumber, int soundID)
    : id ((soundID != 0) ? soundID : generateObjectID (++lastSoundIdSeed)) {
    duration.set (0);
    juce::zerostruct (key);
    juce::zerostruct (key.adsr);

    key.note        = noteNumber;
    key.length      = 0;
    key.muted       = false;
    key.reverse     = false;
    key.gain        = 1.0;
    key.pitch       = 0.0;
    key.voiceGroup  = -1;
    key.volume      = Decibels::gainToDecibels (key.gain);
    key.triggerMode = TriggerMode::Retrigger;

    activeLayers.ensureStorageAllocated (32);

    midiChans.setRange (0, 17, true);
    midiNotes.setRange (0, 128, false);
    midiNotes.setBit (key.note);
}

SamplerSound::~SamplerSound() {}

DynamicObject::Ptr SamplerSound::createDynamicObject() const {
    DynamicObject::Ptr object = new DynamicObject();
    jassertfalse;
#if 0
    object->setProperty (Slugs::id, id);
    object->setProperty (Slugs::note, key.note);
    object->setProperty (Slugs::length, key.length);
    object->setProperty (Slugs::volume, (double) key.volume);
    object->setProperty (Tags::voiceGroup, key.voiceGroup);
    object->setProperty (Tags::triggerMode, (int) key.triggerMode);

    var layers;
    for (LayerData* ld : activeLayers) {
        if (DynamicObject::Ptr lo = ld->createDynamicObject())
            layers.append (lo.get());
    }

    if (layers.size() > 0)
        object->setProperty ("layers", layers);
#endif
    return object;
}

bool SamplerSound::insertLayerData (LayerData* data) {
    if (activeLayers.contains (data) || ! juce::isPositiveAndBelow (activeLayers.size(), 32))
        return false;

    data->parent = static_cast<uint32_t> (id);
    data->sound  = this;
    data->note   = key.note;
    data->index  = activeLayers.size() - 1;

    dataLock.lock();
    activeLayers.add (data);
    dataLock.unlock();

    setDefaultLength();
    return true;
}

void SamplerSound::setDefaultLength() {
    start.set (0);

    int64_t begin = 0;
    int64_t end   = 0;

    for (int i = 0; i < activeLayers.size(); ++i) {
        auto* const data = activeLayers.getUnchecked (i);

        if (data->getLength() <= 0) {
            jassertfalse;
        }

        if (i == 0) {
            begin = data->getStart();
            end   = data->getStart() + data->getLength();
        } else {
            if (data->getStart() < begin)
                begin = data->getStart();

            if ((data->getStart() + data->getLength()) > end)
                end = data->getStart() + data->getLength();
        }
    }

    if (activeLayers.size() > 0) {
        // avoid asserting when there are no layers
        jassert (end > begin);
    }

    start.set (begin);
    duration.set (end - begin);
}

bool SamplerSound::appliesToNote (const int note) {
    return note >= key.note && note <= (key.note + key.length);
}

bool SamplerSound::appliesToChannel (const int chan) {
    return chan <= 0 || chan > 16 || midiChans[chan];
}

void SamplerSound::clearSources() {
    Lock sl (*this);
    activeLayers.clearQuick();
}

int SamplerSound::getRootNote() const { return key.note; }

int64_t SamplerSound::longestLayerFrames() const {
    int64_t longest = 0;

    for (const LayerData* src : activeLayers) {
        if (src && src->lengthInSamples > longest)
            longest = src->lengthInSamples;
    }

    return longest;
}

void SamplerSound::removeLayer (LayerData* data) {
    dataLock.lock();
    activeLayers.removeFirstMatchingValue (data);
    dataLock.unlock();
}

void SamplerSound::setMidiChannel (int chan) {
    if (chan >= 1 && chan <= 16) {
        dataLock.lock();
        midiChans.clear();
        midiChans.setBit (chan);
        dataLock.unlock();
    } else {
        dataLock.lock();
        midiChans.setRange (0, 16, true);
        dataLock.unlock();
    }
}

void SamplerSound::setAttack (double ratio) {
    const float len = ratio * ((double) length() / 4.0f);

    Lock sl (*this);
    key.adsr.setAttack (len);
}

void SamplerSound::setDecay (double ratio) {
    const float len = ratio * ((double) length() / 4.0f);

    Lock sl (*this);
    key.adsr.setDecay (len);
}

void SamplerSound::setSustain (double level) {
    level = ksp1::limit (0.0, 1.0, level);
    Lock sl (*this);
    key.adsr.setSustain (level);
}

void SamplerSound::setRelease (double ratio) {
    const float len = ratio * ((double) length() / 4.0f);

    Lock sl (*this);
    key.adsr.setRelease (len);
}

void SamplerSound::restoreFromJSON (const juce::var& json) {
#if 0
    key.volume = (float) json.getProperty (Slugs::volume, 0.0);
    key.gain = Decibels::decibelsToGain (key.volume);
    key.length = (int) json.getProperty (Slugs::length, key.length);
    key.note = (int) json.getProperty (Slugs::note, key.note);
    key.pitch = (double) json.getProperty (Slugs::pitch, key.pitch);
    key.voiceGroup = (int) json.getProperty (Tags::voiceGroup, -1);
    key.triggerMode = (int) json.getProperty (Tags::triggerMode, (int) TriggerMode::Retrigger);
#endif
}

void SamplerSound::setRootNote (int note) {
    jassert (isPositiveAndBelow (note, 128));
    key.note  = note;
    int index = 0;

    for (LayerData* layer : activeLayers) {
        if (layer) {
            layer->note  = key.note;
            layer->index = index++;
        }
    }
}

#if defined(HAVE_LVTK)
void SamplerSound::setProperty (const URIs& uris, const PatchSet& set) {
    const uint32_t prop (set.property.as_urid());

    if (uris.slugs_voiceGroup == prop) {
        key.voiceGroup = set.value.as_int();
    } else if (uris.slugs_triggerMode == prop) {
        key.triggerMode = set.value.as_int();
    } else if (uris.slugs_length == prop) {
        key.length = set.value.as_int();
    } else if (prop == uris.slugs_note) {
        key.note = set.value.as_int();
    } else if (prop == uris.slugs_pitch) {
        key.pitch = set.value.as_double();
    } else if (prop == uris.slugs_volume) {
        setVolume (set.value.as_double());
    }
}

ForgeRef SamplerSound::writeAtomObject (Forge& forge) {
    const URIs& uris (forge.uris);
    ForgeFrame frame;

    ForgeRef ref = forge.write_object (frame, static_cast<uint32_t> (id), uris.ksp1_SamplerSound);
    forge.write_key (uris.slugs_note);
    forge.write_int (key.note);
    forge.write_key (uris.slugs_length);
    forge.write_int (key.length);
    forge.write_key (uris.slugs_volume);
    forge.write_double (key.volume);
    forge.write_key (uris.slugs_pitch);
    forge.write_double (key.pitch);
    forge.write_key (uris.slugs_triggerMode);
    forge.write_int (static_cast<int> (key.triggerMode));
    forge.write_key (uris.slugs_voiceGroup);
    forge.write_int (key.voiceGroup);
    forge.pop_frame (frame);

    return ref;
}
#endif

} // namespace ksp1
