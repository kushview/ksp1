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

#include "Instrument.h"
#include "KSP1.h"
#include "engine/SamplerSounds.h"

using namespace kv;

namespace KSP1 {

KeyItem::KeyItem (bool setMissing)
    : ObjectModel (Tags::key) {
    nlayers = 0;
    if (setMissing)
        setMissingProperties();
}

KeyItem::KeyItem (const int note, const bool setMissing)
    : ObjectModel (Tags::key),
      nlayers (0) {
    objectData.setProperty (Slugs::note, note, nullptr);
    if (setMissing)
        setMissingProperties();
}

KeyItem::KeyItem (const ValueTree& k)
    : ObjectModel (k), nlayers (0)

{
    setMissingProperties();
    nlayers = countLayers();
}

SamplerSound* KeyItem::getObject() const {
    return dynamic_cast<SamplerSound*> (getProperty ("object").getObject());
}

LayerItem KeyItem::addLayer (const File& file) {
    const int layerIndex = countLayers();
    ValueTree layerNode (Tags::layer);

    layerNode.setProperty (Slugs::index, layerIndex, nullptr);
    layerNode.setProperty (Slugs::file, file.getFullPathName(), nullptr);
    layerNode.setProperty (Slugs::name, file.getFileNameWithoutExtension(), nullptr);
    layerNode.setProperty (Slugs::note, getNote(), nullptr);

    LayerItem layer (layerNode);
    objectData.addChild (layerNode, layerIndex, nullptr);

    return layer;
}

LayerItem KeyItem::addLayer (const LayerItem& layer) {
    int layerIndex  = countLayers();
    ValueTree lnode = layer.node().createCopy();
    lnode.setProperty (Slugs::index, layerIndex, nullptr);
    lnode.setProperty (Slugs::note, getNote(), nullptr);
    LayerItem item (lnode);
    objectData.addChild (lnode, layerIndex, nullptr);

    return item;
}

void KeyItem::clearLayers() {
    for (int32 i = getNumChildren(); --i >= 0;) {
        ValueTree vt = objectData.getChild (i);
        if (vt.hasType (Tags::layer)) {
            objectData.removeChild (vt, nullptr);
        }
    }
}

int KeyItem::countLayers() const {
    if (! isValid())
        return 0;

    int count = 0;
    for (int i = objectData.getNumChildren(); --i >= 0;)
        if (objectData.getChild (i).hasType (Tags::layer))
            ++count;
    return count;
}

LayerItem KeyItem::getLayer (int index) const {
    for (int i = objectData.getNumChildren(); --i >= 0;) {
        LayerItem layer (objectData.getChild (i));
        if (layer.isValid() && layer.index() == index)
            return layer;
    }

    return LayerItem (ValueTree::invalid);
}

void KeyItem::removeLayer (int index) {
    LayerItem layer (getLayer (index));
    if (layer.isValid()) {
        const bool updateIndexes = (layer.index() < countLayers());
        objectData.removeChild (layer.node(), nullptr);
#if 1
        if (updateIndexes) {
            int index = 0;
            for (int i = 0; i < objectData.getNumChildren(); ++i) {
                ValueTree v (objectData.getChild (i));
                if (! v.hasType (Tags::layer))
                    continue;

                LayerItem l (v);
                if (l.isValid()) {
                    if (l.index() != index)
                        l.node().setProperty (Slugs::index, index, nullptr);
                    ++index;
                }
            }
        }
#endif
    }
}

void KeyItem::set (const Identifier& k, const var& v) {
    objectData.setProperty (k, v, nullptr);
}

void KeyItem::setMissingProperties() {
    Random rand (Time::currentTimeMillis());
    stabilizePropertyPOD (Slugs::id, rand.nextInt (Range<int> (1, std::numeric_limits<int>::max())));
    stabilizePropertyPOD (Slugs::note, (int) -1);
    stabilizePropertyPOD (Slugs::length, (int) 0);
    stabilizePropertyPOD (Slugs::volume, (double) 0.0f);
    stabilizePropertyPOD (Tags::triggerMode, (int) TriggerMode::Retrigger);
    stabilizePropertyPOD (Tags::voiceGroup, (int) -1);
    stabilizePropertyPOD (Slugs::attack, (double) 0.05);
    stabilizePropertyPOD (Slugs::decay, (double) 0.05);
    stabilizePropertyPOD (Slugs::sustain, (double) 0.5);
    stabilizePropertyPOD (Slugs::release, (double) 0.4);

    const int nlayers = countLayers();
    for (int i = 0; i < nlayers; ++i) {
        LayerItem layer (getLayer (i));
        layer.setMissingProperties();
    }
}

void KeyItem::setAdsr (float a, float d, float s, float r) {
    set (Slugs::attack, a);
    set (Slugs::decay, d);
    set (Slugs::sustain, s);
    set (Slugs::release, r);
}

KeyItem& KeyItem::operator= (const KeyItem& other) {
    nlayers          = other.nlayers;
    this->objectData = other.objectData;
    return *this;
}

#if 0
    // TOOD: support LV2
    void KeyItem::accept (lvtk::AtomObject key, fish::SamplerURIs& uris)
    {
        LV2_Atom* volume  = nullptr, *note = nullptr;
        LV2_Atom* trigger = nullptr, *muteGroup = nullptr;

        lv2_atom_object_get (key,
               (uint32) uris.param_index, &note,
               (uint32) uris.param_volume, &volume,
               (uint32) uris.prop_muteGroup, &muteGroup,
               (uint32) uris.prop_mode, &trigger, nullptr);

        if (note)
        {
           const int n = lvtk::Atom(note).as_int();

           if (volume)
           {
               set ("volume", lvtk::Atom(volume).as_float());
               std::clog << "v: " << this->volume() << std::endl;
           }
           if (trigger)
           {
               set ("trigger-mode", lvtk::Atom(trigger).as_int());
               std::clog << "t: " << this->triggerMode() << std::endl;
           }
           if (muteGroup)
           {
               set ("mute-group", lvtk::Atom(muteGroup).as_int());
               std::clog << "mg: " << this->muteGroup() << std::endl;
           }
        }
    }
#endif
} // namespace KSP1
