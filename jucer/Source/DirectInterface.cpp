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

#if 1

#include "engine/LayerSource.h"
#include "engine/SamplerSounds.h"
#include "DirectInterface.h"
#include "Instrument.h"

namespace KSP1 {

    DirectInterface::DirectInterface (SampleCache& c)
        : SamplerSynth (c),
          uri ("ksp1://interface/direct")
    {

    }

    DirectInterface::~DirectInterface()
    {

    }

    void DirectInterface::clearKeyboard()
    {
        SamplerSynth::clearAllSounds();
    }


    bool DirectInterface::addLayer (const LayerItem& layer)
    {
        SamplerSound* sound = getSound (layer.getNote());
        LayerData* source = nullptr; //createLayerSource (layer);

        if (! sound) {
            jassertfalse;
        }

        if (! source) {
            jassertfalse;
        }
#if 0
        //FIXME
        if (source && sound && source->load (File (layer.fileString())))
        {
            layers.addIfNotAlreadyThere (source);
            //FIXME: sound->addLayer (source);
            return true;
        }
#endif

        jassertfalse;

        if (source) {
            delete source;
            source = nullptr;
        }

        return false;
    }

    void
    DirectInterface::mutate (const Instrument& instrument, const Identifier& prop)
    {
        if (prop == Slugs::volume)
        {
            setMasterVolume (instrument.getVolume());
        }
    }

    void
    DirectInterface::mutate (const KeyItem& key, const Identifier& prop)
    {
        if (! key.isValid())
            return;

        SamplerSound* sound = getSound (key.note(), false);
        if (! sound)
            return;

        if (prop == Slugs::volume)
        {
            sound->setVolume (key.volume());
        }
        else if (prop == Slugs::length) {
            const int length = (int) key.getProperty (Slugs::length);
            sound->setKeyLength (length);
        }
        else if (prop == Slugs::attack)
        {
            const double attack = key.getProperty (Slugs::attack);
            sound->setAttack (attack);
        }
        else if (prop == Slugs::decay)
        {
            const double decay = key.getProperty (Slugs::decay);
            sound->setDecay (decay);
        }
        else if (prop == Slugs::sustain)
        {
            const double sustain = key.getProperty (Slugs::sustain);
            sound->setSustain (sustain);
        }
        else if (prop == Slugs::release)
        {
            const double release = key.getProperty (Slugs::release);
            sound->setRelease (release);
        }
        else if (prop == Tags::voiceGroup)
        {
            const int32 vg = Element::clampBelow (key.voiceGroup(), -1, 8);
            sound->setVoiceGroup (vg);
        }
        else if (prop == KSP1::fxSendIdentifier (1))
        {
            float value = key.getProperty (prop, 0.0f);
            sound->setFxSendValue (0, value);
        }
        else if (prop == KSP1::fxSendIdentifier (2))
        {
            float value = key.getProperty (prop, 0.0f);
            sound->setFxSendValue (1, value);
        }
        else if (prop == KSP1::fxSendIdentifier (3))
        {
            float value = key.getProperty (prop, 0.0f);
            sound->setFxSendValue (2, value);
        }
        else if (prop == KSP1::fxSendIdentifier (4))
        {
            float value = key.getProperty (prop, 0.0f);
            sound->setFxSendValue (3, value);
        }
        else if (prop == Tags::triggerMode)
        {
            jassert (isPositiveAndBelow (key.triggerMode(), TriggerMode::numModes));
            const TriggerMode tmode (key.triggerMode());
            sound->setTriggerMode (tmode);
        }
    }

    void DirectInterface::mutate (const LayerItem& layer, const Identifier& prop)
    {
        if (! layer.isValid() && prop.isValid())
            return;

        SamplerSound* sound = getSound (layer.getNote(), false);
        if (! sound)
            return;

        LayerData* source = nullptr;
        for (LayerData* src : *sound)
        {
#if 0
            //FIXME:
            if (src->state == layer.node())
            {
                source = src;
                break;
            }
#endif
        }

        if (! source)
            return;

        if (prop == Slugs::volume)
        {
            source->gain.set (layer.gain());
        }
        else if (prop == Identifier ("in-point"))
        {
            int64 t (layer.startTime() * 44100.0f);
            source->in.set (t);
        }
        else if (prop == Identifier ("out-point"))
        {
            int64 t (layer.endTime() * 44100.0f);
            source->out.set (t);
        }
        else if (prop == Tags::panning)
        {
            source->panning.set (layer.panning());
        }
        else if (prop == KSP1::pitchSlug)
        {
            source->pitch.set (layer.pitch());
        }
        else if (prop == KSP1::cutoffSlug)
        {
            source->cutoff.set (layer.cutoff());
        }
        else if (prop == KSP1::resonanceSlug)
        {
            source->resonance.set (layer.resonance());
        }
        else if (prop == KSP1::velocityLower || prop == KSP1::velocityUpper)
        {
            SamplerSound::Lock sl (*sound);
            layer.getVelocityRange (source->velocityRange);
        }
    }

    bool
    DirectInterface::removeLayer (const LayerItem& layer)
    {
        if (SamplerSound* sound = getSound (layer.getNote(), false))
        {
            for (LayerData* source : *sound)
            {
#if 0
                //FIXME:
                if (source->state == layer.node())
                {
                    sound->removeLayer (source);
                    layers.removeObject (source);
                    return true;
                }
#endif
            }
        }

        jassertfalse;
        return false;
    }

#if 0
    void DirectInterface::setInstrument (InstrumentPtr inst)
    {
        assert (inst != nullptr);
        instrument = inst;
        setState (instrument->node());
    }
    
    void DirectInterface::setState (const ValueTree &data)
    {
        state = data;
    }

    void DirectInterface::valueTreePropertyChanged (ValueTree& tree, const Identifier& property)
    {
        if (tree == state)
        {
            Instrument inst (tree);
            this->mutate (inst, property);
        }
        else if (tree.hasType (Tags::key))
        {
            KeyItem key (tree);
            this->mutate (key, property);
        }
        else if (tree.hasType (Tags::layer))
        {
            LayerItem layer (tree);
            this->mutate (layer, property);
        }
    }

    void DirectInterface::valueTreeChildAdded (ValueTree& parent, ValueTree& child)
    {
        if (child.hasType (Tags::key))
        {
            KeyItem key (child);
        }
        else if (child.hasType (Tags::layer))
        {
            LayerItem layer (child);
            if (layer.isValid())
                this->addLayer (layer);
        }
    }

    void DirectInterface::valueTreeChildRemoved (ValueTree& parent, ValueTree& child)
    {
        if (child.hasType (Tags::key))
        {
            KeyItem key (child);
        }
        else if (child.hasType (Tags::layer))
        {
            LayerItem layer (child);
            if (layer.isValid())
                this->removeLayer (layer);
            else
            {

            }
        }
    }

    void DirectInterface::valueTreeChildOrderChanged (ValueTree& /*tree*/)
    {

    }

    void DirectInterface::valueTreeParentChanged (ValueTree& /*tree*/)
    {

    }

    void DirectInterface::valueTreeRedirected (ValueTree& tree)
    {
        if (tree != state)
            return;

        reloadState();
    }
#endif

    void DirectInterface::reloadState()
    {
        clearKeyboard();
        InstrumentPtr i (getInstrument());

        for (int note = 0; note < 128; ++note)
        {
            KeyItem key (i->getKey (note, false));
            ValueTree kv = key.node();
            const SamplerSound* const sound = getSound (key.note());
            (void) sound;

            for (int i = kv.getNumProperties(); --i >= 0;)
            {
                const Identifier prop = kv.getPropertyName (i);
                mutate (key, prop);
            }

            for (int l = 0; l < key.countLayers(); ++l)
            {
                LayerItem layer (key.getLayer (l));
                addLayer (layer);

                ValueTree ld = layer.node();
                for (int32 i = ld.getNumProperties(); --i >= 0;)
                {
                    const Identifier prop = ld.getPropertyName (i);
                    mutate (layer, prop);
                }
            }
        }
    }
}
#endif

