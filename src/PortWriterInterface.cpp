/*
    PortWriterInterface.cpp - This file is part of KSP1
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

#include "PortWriterInterface.h"
#include "Ports.h"
#include "PortWriter.h"

namespace KSP1 {

PortWriterInterface::PortWriterInterface (PortWriter& w)
    : writer (w)
{
    buffer.setSize (1024);
}

PortWriterInterface::~PortWriterInterface()
{
}

void PortWriterInterface::handleMidi (const MidiMessage &midi) {
    if (frozen)
        return;

    writer.writeMidi (Port::AtomInput, midi);
}

bool PortWriterInterface::addKey (const KeyItem& key)
{
    if (frozen)
        return true;

    Forge& forge (writer.getForge (buffer));
    const LV2_Atom_URID _subject = { { sizeof(LV2_URID), forge.uris.atom_URID },
                                      forge.uris.ksp1_Instrument };
    const lvtk::Atom subject (&_subject);
    const lvtk::Atom value (key.writeAtomObject (forge));
    writer.addProperty (subject, forge.uris.slugs_key, value);
    return true;
}

bool PortWriterInterface::removeKey (const KeyItem& key)
{
    if (frozen)
        return true;

    return true;
}

bool PortWriterInterface::addLayer (const LayerItem& layer)
{
    if (frozen)
        return true;

    Forge& forge (writer.getForge (buffer));
    const KeyItem parent (layer.getParentValueTree());
    const lvtk::Atom subject (parent.writeAtomSubject (forge));
    const lvtk::Atom value (layer.writeAtomObject (forge));
    writer.addProperty (subject, forge.uris.slugs_layer, value);
    return true;
}

void PortWriterInterface::loadFile (const File& file)
{
    if (frozen)
        return;

    if (file.hasFileExtension (".xml"))
        writer.setInstrument (file);
}

bool PortWriterInterface::removeLayer (const LayerItem& layer)
{
    if (frozen)
        return true;

    Forge& f = writer.getForge (buffer);
    writer.patchDelete (layer.writeAtomObject (f));
    return true;
}

void PortWriterInterface::clearKeyboard()
{
    if (frozen)
        return;

    DBG ("AtomPortInterface::clearKeyboard");
}

void PortWriterInterface::mutate (const Instrument& i, const Identifier& prop)
{
    if (frozen)
        return;

    Forge& f = writer.getForge (buffer);
    writer.setObjectProperty (
        f.uris.ksp1_Instrument,
        URIs::slugToURID (f, prop.toString().toRawUTF8()),
        i.getProperty (prop), false
    );
}

void PortWriterInterface::mutate (const KeyItem& k, const Identifier& prop)
{
    if (frozen)
        return;

    if (prop == Slugs::note) {

    }

    Forge& f = writer.getForge (buffer);
    const lvtk::Atom subject (k.writeAtomSubject (f));
    writer.setObjectProperty (
        subject.cobj(),
        URIs::slugToURID (f, prop.toString().toRawUTF8()),
        k.getProperty (prop), true
    );
}

void PortWriterInterface::mutate (const LayerItem& l, const Identifier& prop)
{
    if (frozen)
        return;

    Forge& f = writer.getForge (buffer);
    const lvtk::Atom subject (l.writeAtomSubject (f));

    writer.setObjectProperty (
        subject.cobj(),
        URIs::slugToURID (f, prop.toString().toRawUTF8()),
        l.getProperty (prop), false
    );
}

}
