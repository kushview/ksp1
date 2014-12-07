/*
    AtomPortInterface.h - This file is part of KSP1
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

#ifndef KSP1_ATOM_PORT_INTERFACE_H
#define KSP1_ATOM_PORT_INTERFACE_H

#include "SamplerInterface.h"

namespace KSP1 {

class PortWriter;

class PortWriterInterface : public SamplerInterface
{
public:
    PortWriterInterface (PortWriter&);
    ~PortWriterInterface();

    void setFrozen (const bool freeze) { frozen = freeze; }
    void handleMidi (const MidiMessage &midi) override;

    void getKeyForNote (const int note) override;

protected:
    bool addKey (const KeyItem&) override;
    bool removeKey (const KeyItem&) override;
    bool addLayer (const LayerItem&) override;
    bool removeLayer (const LayerItem&) override;
    void clearKeyboard();

    void loadFile (const File &) override;

    void mutate (const Instrument& key, const Identifier& prop) override;
    void mutate (const KeyItem& key, const Identifier& prop) override;
    void mutate (const LayerItem& key, const Identifier& prop) override;

private:
    const String uri;
    PortWriter& writer;
    MemoryBlock buffer;
    bool frozen;
};

}

#endif // KSP1_ATOM_PORT_INTERFACE_H
