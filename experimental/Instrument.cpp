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
#include "BeatThangKitLoader.h"
#include "StandardLoader.h"

namespace KSP1 {

    struct KeySorter
    {
        explicit KeySorter (bool asc = false) : ascending (asc) { }

        int compareElements (const ValueTree& left, const ValueTree& right)
        {
            if ((left.hasType (Tags::key)) && (! right.hasType (Tags::key))) {
                return 1;
            } else if ((! left.hasType (Tags::key)) && (right.hasType (Tags::key))) {
                return -1;
            } else if ((! left.hasType (Tags::key)) && (! right.hasType (Tags::key))) {
                return -1;
            }

            const int noteL = left.getProperty (Slugs::note);
            const int noteR = right.getProperty (Slugs::note);

            if (noteL < noteR)
                return (ascending) ? 1 : -1;
            else if (noteL > noteR)
                return (ascending) ? -1 : 1;

            jassert (noteL == noteR);

            const int lenL  = left.getProperty (Slugs::length);
            const int lenR  = right.getProperty (Slugs::length);

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
        : MediaObject ("instrument")
    {
        selectedNote = 0;
        setName (name);
    }

    Instrument::Instrument (const ValueTree& data)
        : MediaObject (data)
    {
        selectedNote = 0;
    }
    
    Instrument::Instrument (const Instrument& other)
        : MediaObject (other.node()),
          selectedNote (other.selectedNote)
    { }
    
    Instrument::~Instrument() { }


    Instrument& Instrument::operator= (const Instrument& other)
    {
        setData (other.node());
        this->selectedNote = other.selectedNote;
        this->file = other.file;
        return *this;
    }
    
    KeyItem Instrument::addKey (int32 noteNumber, int32 keySpan)
    {
        if (! isPositiveAndBelow (noteNumber, 128))
            return KeyItem();
        
        bool wasJustCreated = false;
        
        ValueTree key = node().getChildWithProperty (Slugs::note, noteNumber);
        if (! key.isValid())
        {
            key = ValueTree ("key");
            key.setProperty (Slugs::note, noteNumber, nullptr);
            if (keySpan > 0)
                key.setProperty (Slugs::length, keySpan, nullptr);
            wasJustCreated = true;
        }

        KeyItem item (key);
        if (wasJustCreated) {
            item.setMissingProperties();
        }
        
        if (key.isValid()) {
            objectData.addChild (key, noteNumber, nullptr);
            KeySorter sorter;
            objectData.sort (sorter, nullptr, true);
        }

        return item;
    }
    
    void Instrument::clear()
    {
        objectData.removeAllChildren (nullptr);
        file = File::nonexistent;
    }

    void Instrument::sortKeys() {
        KeySorter s; objectData.sort (s, nullptr, true);
    }

    String Instrument::getName() const
    {
        return getProperty (Slugs::name);
    }

    void Instrument::setMissingProperties (bool recursive)
    {
        ValueTree root (node());

        if (! objectData.hasProperty (Slugs::name)) {
            objectData.setProperty (Slugs::name, "Unknown Instrument", 0);
        }

        if (! objectData.hasProperty (Slugs::volume)) {
            objectData.setProperty (Slugs::volume, (double)1.0, 0);
        }

        if (! recursive)
            return;

        for (int i = 0; i < root.getNumChildren(); ++i)
        {
            ValueTree child (root.getChild(i));
            if (child.hasType (Tags::key))
            {
                KeyItem key (child);
                key.setMissingProperties();
            }
        }
    }

    void Instrument::updateKeyItem (const URIs& uris, const lvtk::AtomObject &object)
    {
        if (object.id() != 0)
        {
            KeyItem key (objectData.getChildWithProperty (Slugs::id, static_cast<int> (object.id())));
            if (key.isNotValid()) {
                key = KeyItem (false);
                objectData.addChild (key.node(), -1, nullptr);
            }

            if (key.isValid())
                key.setAtomObject (uris, object);

            return;
        }

        jassertfalse; // this is old code
        const lvtk::Atom note;
        lv2_atom_object_get (object, uris.slugs_note, &note, 0);
        if (note)
        {
            KeyItem key (getKey (note.as_int()));
            key.setAtomObject (uris, object);
        }
        else
        {
            DBG ("can't update key item");
        }
    }

    void Instrument::updateLayerItem (const URIs& uris, const lvtk::AtomObject &object)
    {
        const int layerId = static_cast<int> (object.id());
        const lvtk::Atom parent;
        lv2_atom_object_get (object, uris.slugs_parent, &parent, 0);

        if (layerId != 0 && ! parent)
        {
            for (int i = 0; i < objectData.getNumChildren(); ++i)
            {
                ValueTree keyData = objectData.getChild (i);
                ValueTree layerData = keyData.getChildWithProperty(Slugs::id, layerId);
                if (layerData.hasType (Tags::layer))
                {
                    LayerItem layer (layerData);
                    layer.setAtomObject (uris, object);
                }
            }

            return;
        }
        else if (layerId != 0 && parent)
        {
            KeyItem key (objectData.getChildWithProperty (Slugs::id, parent.as_int()));
            if (key.isValid())
            {
                ValueTree layerData = key.node().getChildWithProperty (Slugs::id, layerId);
                if (! layerData.isValid())
                {

                    layerData = ValueTree (Tags::layer);
                    layerData.setProperty (Slugs::id, layerId, nullptr);
                    key.node().addChild (layerData, 0, nullptr);
                    LayerItem layer (layerData);
                    layer.setAtomObject (uris, object);
                }
                else
                {
                    LayerItem layer (layerData);
                    layer.setAtomObject (uris, object);
                }
            }

            return;
        }

        jassertfalse; //TODO: the following is old code, left for reference

#if 0
        const lvtk::Atom index, note;
        lv2_atom_object_get (object, uris.slugs_index, &index,
                                     uris.slugs_note, &note, 0);
        if (note && index)
        {
            const KeyItem key = getKey (note.as_int());
            LayerItem layer (key.getLayer (index.as_int()));
            layer.setAtomObject (uris, object);
        }
        else
        {
            DBG ("can't update layer item");
        }
#endif
    }

    KeyItem Instrument::getKey (int32 noteNumber, bool setMissing) const
    {
        if (! isPositiveAndBelow (noteNumber, 128))
            return KeyItem();

        KeyItem key (objectData.getChildWithProperty (Slugs::note, noteNumber));
        if (setMissing)
            key.setMissingProperties();

        return key;
    }

    KeyItem Instrument::getOrCreateKey (int32 noteNumber)
    {
        KeyItem existing (getKey (noteNumber));
        if (existing.isValid())
            return existing;

        return addKey (noteNumber);
    }

    LayerItem Instrument::getLayer (int32 key, int32 layer)
    {
        KeyItem item (getKey (key));
        return item.getLayer (layer);
    }

    XmlElement* Instrument::createXml()
    {
        return objectData.createXml();
    }

    void Instrument::getSamples (StringArray& samples) const
    {
        const ValueTree& node (objectData);
        for (int i = 0; i < node.getNumChildren(); ++i) {
            const ValueTree key (node.getChild (i));
            for (int j = 0; j < key.getNumChildren(); ++j) {
                const ValueTree layer (key.getChild (j));
                if (layer.hasProperty (Slugs::file)) {
                    samples.addIfNotAlreadyThere (layer.getProperty (Slugs::file));
                }
            }
        }
    }

    void Instrument::loadFromXml (const XmlElement& xml)
    {
        ValueTree newData (ValueTree::fromXml (xml));
        if (newData.hasType ("instrument"))
            setData (newData);
    }

    void
    Instrument::load (const Programming::Item& program, ProgressSink& sink)
    {
        this->load (program.asFile(), sink);
    }

    void
    Instrument::load (const File& file, ProgressSink& sink)
    {
        ScopedPointer<InstrumentLoader> loader = nullptr;

        if (file.getFileExtension() == ".xml") {
            loader = new KSP1::XmlLoader (*this, sink);
        }
        else if (file.getFileExtension() == ".btdk") {
            loader = new BeatThangKitLoader (*this, sink);
        }

        if (loader != nullptr)
        {
            loader->setSink (sink);
            
            for (int k = 0; k < 128; ++k)
                getKey(k).clearLayers();

            loader->loadFile (file);
            this->file = file;
        }
    }


    void
    Instrument::setName (const String& name)
    {
        getPropertyAsValue (Slugs::name) = name;
    }

}
