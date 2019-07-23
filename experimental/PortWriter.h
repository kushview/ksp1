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


#ifndef KSP1_PORT_WRITER_H
#define KSP1_PORT_WRITER_H

#include <functional>
#include "KSP1.h"
#include "Forge.h"
#include "Ports.h"
#include "Instrument.h"

namespace KSP1 {

    class PortWriter
    {
    public:

        typedef std::function<void(uint32_t, uint32_t, uint32_t, void const*)> WriteFunc;
        PortWriter (Forge& f, WriteFunc w)
            : forge (f), write (w)
        {
            bufferSize = 0;
        }

        Forge& getForge (const bool prepare = true)
        {
            if (prepare)
                prepareForge();
            return forge;
        }

        Forge& getForge (MemoryBlock& buffer) {
            forge.set_buffer ((uint8*) buffer.getData(), buffer.getSize());
            return forge;
        }

        inline void setObjectProperty (uint32 subjectURID, uint32 property, const var& value, bool prep = false)
        {
            Forge& forge (getForge (false));
            LV2_Atom_URID subject = { { sizeof(LV2_URID), forge.URID }, subjectURID };
            setObjectProperty ((LV2_Atom*) &subject, property, value, prep);
        }

        inline void setObjectProperty (const LV2_Atom* subject, uint32 property, const var& value, bool prep = false)
        {
            Forge& f = getForge (prep);
            lvtk::Atom atom;

            if (value.isBool()) {
                atom = f.patch_set_bool (property, (bool) value, subject);
            } else if (value.isInt()) {
                atom = f.patch_set_int (property, (int) value, subject);
            } else if (value.isInt64()) {
                atom = f.patch_set_long (property, (int64) value, subject);
            } else if (value.isDouble()) {
                atom = f.patch_set_double (property, (double) value, subject);
            } else if (value.isString()) {
                atom = f.patch_set_string (property, value.toString().toRawUTF8(), f.String, subject);
            }

            if (atom)
                writeAtom (Port::AtomInput, atom);
        }

        inline void addLayerToNote (int note, const File& file)
        {
            prepareForge();
        }

        inline void addProperty (const lvtk::Atom& subject, uint32_t property, const lvtk::Atom& value)
        {
            prepareForge();
            const lvtk::Atom atom (forge.patch_add_single (subject, property, value));

            if (atom)
                writeAtom (Port::AtomInput, atom);
        }

        inline void patchDelete (const lvtk::Atom& subject) {
            prepareForge();
            const lvtk::Atom atom (forge.patch_delete (subject));
            if (atom)
                writeAtom (Port::AtomInput, atom);
        }

        inline void keyVolume (int keyid, float volume)
        {
            prepareForge();
        }

        inline void setInstrument (const File& file)
        {
            prepareForge();
            const lvtk::Atom subject (forge.write_urid (forge.uris.ksp1_Instrument));
            const lvtk::Atom setPath (forge.patch_set_path (forge.uris.slugs_file,
                                      file.getFullPathName().toRawUTF8(), subject));
            writeAtom (Port::AtomInput, setPath);
        }

        inline void keyTriggerMode (int keyid, int trigger)
        {

        }

        inline void keyMuteGroup (int keyid, int grp)
        {
#if 0
            lvtk::AtomForge& f = getForge();
            uint8_t b[128];
            f.set_buffer (b, 128);
            lvtk::ForgeFrame frame;

            lvtk::Atom atom (f.blank (frame, uint32_t (1 + keyid), sampler_Key));

            f.property_head (param_index, 0);
            f.write_int (keyid);

            f.property_head (prop_muteGroup, 0);
            f.write_int (grp);

            f.pop (frame);

            writeAtom (sampler_control, atom);
#endif
        }

        inline void writeMidi (uint32_t port, const uint8_t* data, uint32_t size)
        {
            prepareForge();
            lvtk::Atom midi (forge.write_midi (data, size));
            writeAtom (port, midi);
        }

        inline void writeMidi (uint32_t port, const MidiMessage& msg) {
            writeMidi (port, msg.getRawData(), static_cast<uint32_t> (msg.getRawDataSize()));
        }

        inline void writeAtom (uint32_t port, const lvtk::Atom& atom) {
            write (port, atom.total_size(), forge.uris.atom_eventTransfer, (void const*) atom.cobj());
        }

        inline void patchGet (uint32_t subject, uint32_t id = 0, uint32_t context = 0)
        {
            prepareForge();
            ForgeRef atom (forge.write_patch_get (subject, id, context));
            writeAtom (Port::AtomInput, atom);
        }

        inline void getLayers (int keyid, uint32_t context)
        {
           // if (keyid >= 0)
            //    patchGet (sampler_Layers, (uint32_t) keyid);
        }


        inline void
        getKey (int keyid, uint32_t context)
        {
            if (! isPositiveAndBelow (keyid, 128))
                return;

            prepareForge();
            const KeyItem tempKey (keyid, false);
            const lvtk::Atom subject (tempKey.writeAtomSubject (forge));
            const lvtk::Atom get (forge.write_patch_get (subject.cobj(), 0, 0));
            writeAtom  (Port::AtomInput, get);
        }

    private:
        Forge& forge;
        WriteFunc write;
        size_t bufferSize;
        HeapBlock<uint8> buffer;

        inline void prepareForge()
        {
            if (bufferSize == 0) {
                bufferSize = 4096;
                buffer.allocate (bufferSize, true);
            }
            forge.set_buffer (buffer.getData(), bufferSize);
        }
    };

}

#endif  /* KSP1_PORT_WRITER_H */
