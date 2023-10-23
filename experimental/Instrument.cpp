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
#if 0
#    include "BeatThangKitLoader.h"
#    include "StandardLoader.h"
#endif

namespace KSP1 {

struct KeySorter {
    explicit KeySorter (bool asc = false) : ascending (asc) {}

    int compareElements (const ValueTree& left, const ValueTree& right) {
        if ((left.hasType (Tags::key)) && (! right.hasType (Tags::key))) {
            return 1;
        } else if ((! left.hasType (Tags::key)) && (right.hasType (Tags::key))) {
            return -1;
        } else if ((! left.hasType (Tags::key)) && (! right.hasType (Tags::key))) {
            return -1;
        }

        const int noteL = left.getProperty (kv::Slugs::note);
        const int noteR = right.getProperty (kv::Slugs::note);

        if (noteL < noteR)
            return (ascending) ? 1 : -1;
        else if (noteL > noteR)
            return (ascending) ? -1 : 1;

        jassert (noteL == noteR);

        const int lenL = left.getProperty (kv::Slugs::length);
        const int lenR = right.getProperty (kv::Slugs::length);

        if (lenL < lenR)
            return -1;
        else if (lenL > lenR)
            return 1;

        return 0;
    }

private:
    bool ascending;
};

Instrument::Instrument (const String& name)
    : MediaObject ("instrument") {
    selectedNote = 0;
    setName (name);
}

Instrument::Instrument (const ValueTree& data)
    : MediaObject (data) {
    selectedNote = 0;
}

Instrument::Instrument (const Instrument& other)
    : MediaObject (other.node()),
      selectedNote (other.selectedNote) {}

Instrument::~Instrument() {}

Instrument& Instrument::operator= (const Instrument& other) {
    objectData         = other.objectData;
    this->selectedNote = other.selectedNote;
    this->file         = other.file;
    return *this;
}

static bool containsSound (const ValueTree& tree, int note, int span) {
    for (int i = 0; i < tree.getNumChildren(); ++i) {
        const auto child = tree.getChild (i);
        if ((int) child[Tags::note] == note && span == (int) child[Tags::length])
            return true;
    }
    return false;
}

KeyItem Instrument::getSound (int index) const {
    KeyItem sound (objectData.getChild (index));
    return sound;
}

void Instrument::setActiveSoundIndex (int index) {
    const auto sound = getSound (index);
    if (sound.isValid())
        setProperty ("activeSound", sound.getProperty (Tags::id));
}

KeyItem Instrument::getActiveSound() const {
    const auto soundId = (int) getProperty ("activeSound");
    if (soundId < 0)
        return getSound (0);
    KeyItem sound (objectData.getChildWithProperty (Tags::id, soundId));
    return sound;
}

void Instrument::removeSound (const KeyItem& item) {
    if (objectData.indexOf (item.getValueTree()))
        ;
    objectData.removeChild (item.getValueTree(), nullptr);
}

KeyItem Instrument::addKey (int32 noteNumber, int32 keySpan) {
    if (! isPositiveAndBelow (noteNumber, 128))
        return KeyItem();

    bool wasJustCreated = false;

    ValueTree key = ValueTree (Tags::key);
    key.setProperty (kv::Slugs::note, noteNumber, nullptr);
    if (keySpan > 0)
        key.setProperty (kv::Slugs::length, keySpan, nullptr);

    KeyItem item (key);
    item.setMissingProperties();

    if (key.isValid()) {
        objectData.addChild (key, noteNumber, nullptr);
    }

    return item;
}

void Instrument::clear() {
    objectData.removeAllChildren (nullptr);
    file = File();
}

void Instrument::sortKeys() {
    KeySorter s;
    objectData.sort (s, nullptr, true);
}

String Instrument::getName() const {
    return getProperty (Tags::name);
}

void Instrument::setMissingProperties (bool recursive) {
    ValueTree root (getValueTree());

    if (! objectData.hasProperty (kv::Slugs::name)) {
        objectData.setProperty (kv::Slugs::name, "Unknown Instrument", 0);
    }

    if (! objectData.hasProperty (kv::Slugs::volume)) {
        objectData.setProperty (kv::Slugs::volume, (double) 1.0, 0);
    }

    if (! recursive)
        return;

    for (int i = 0; i < root.getNumChildren(); ++i) {
        ValueTree child (root.getChild (i));
        if (child.hasType (Tags::key)) {
            KeyItem key (child);
            key.setMissingProperties();
        }
    }
}

KeyItem Instrument::getKey (int32 noteNumber, bool setMissing) const {
    if (! isPositiveAndBelow (noteNumber, 128))
        return KeyItem();

    KeyItem key (objectData.getChildWithProperty (kv::Slugs::note, noteNumber));
    if (setMissing)
        key.setMissingProperties();

    return key;
}

KeyItem Instrument::getOrCreateKey (int32 noteNumber) {
    KeyItem existing (getKey (noteNumber));
    if (existing.isValid())
        return existing;

    return addKey (noteNumber);
}

LayerItem Instrument::getLayer (int32 key, int32 layer) {
    KeyItem item (getKey (key));
    return item.getLayer (layer);
}

XmlElement* Instrument::createXml() {
    return objectData.createXml();
}

void Instrument::getSamples (StringArray& samples) const {
    const ValueTree& node (objectData);
    for (int i = 0; i < node.getNumChildren(); ++i) {
        const ValueTree key (node.getChild (i));
        for (int j = 0; j < key.getNumChildren(); ++j) {
            const ValueTree layer (key.getChild (j));
            if (layer.hasProperty (kv::Slugs::file)) {
                samples.addIfNotAlreadyThere (layer.getProperty (kv::Slugs::file));
            }
        }
    }
}

void Instrument::loadFromXml (const XmlElement& xml) {
    ValueTree newData (ValueTree::fromXml (xml));
    if (newData.hasType (Tags::instrument))
        setData (newData);
}

void Instrument::setName (const String& name) {
    getPropertyAsValue (kv::Slugs::name) = name;
}

} // namespace KSP1
