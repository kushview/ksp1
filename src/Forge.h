/*
    Forge.h - This file is part of KSP1
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

#ifndef KSP1_FORGE_H
#define KSP1_FORGE_H

#include "URIs.h"
#include "../../libs/lvtk/lvtk/ext/atom.hpp"

namespace KSP1
{
    typedef lvtk::Atom Atom;
    typedef lvtk::AtomObject AtomObject;
    typedef lvtk::ForgeRef ForgeRef;
    typedef lvtk::ForgeFrame ForgeFrame;

    class Forge : public lvtk::AtomForge
    {
    public:
        Forge (const URIs& u, LV2_URID_Map* map, LV2_URID_Unmap* unmap)
            : AtomForge (map),  uris (u), p_map (map), p_unmap (unmap)
        { }

        ForgeRef write_midi (const uint8_t* data, uint32_t size)
        {
            ForgeRef midi (lv2_atom_forge_atom (this, size, uris.midi_MidiEvent));
            lv2_atom_forge_raw (this, data, size);
            return midi;
        }

        LV2_URID map (const char* uri)
        {
            assert (p_map && p_map->map);
            return p_map->map (p_map->handle, uri);
        }

        const char* unmap (LV2_URID urid)
        {
            assert (p_unmap && p_unmap->unmap);
            return p_unmap->unmap (p_unmap->handle, urid);
        }

        ForgeRef write_patch_get (const LV2_Atom* subject, uint32_t id = 0, uint32_t context = 0)
        {
            ForgeFrame frame;
            ForgeRef atom (write_object (frame, id, uris.patch_Get));

            write_key (uris.patch_subject);
            write_primitive (subject);
            pop_frame (frame);

            return atom;
        }

        ForgeRef write_patch_get (uint32_t subject_urid, uint32_t id = 0, uint32_t context = 0)
        {
            const LV2_Atom_URID atom = { { sizeof(uint32_t), this->URID }, subject_urid };
            return write_patch_get (reinterpret_cast<const LV2_Atom*> (&atom), id, context);
        }

        ForgeRef patch_delete (const LV2_Atom* subject)
        {
            ForgeFrame frame;
            ForgeRef del = write_object (frame, 0, uris.patch_Delete);
            write_key (uris.patch_subject);
            write_primitive (subject);
            pop_frame (frame);

            return del;
        }

        ForgeRef patch_add_single (const LV2_Atom* subject, const uint32_t property, const LV2_Atom* value, uint32_t id = 0)
        {
            ForgeFrame patchFrame;
            ForgeRef patch = write_object (patchFrame, id, uris.patch_Patch);
            write_key (uris.patch_subject);
            write_primitive (subject);

            ForgeFrame addFrame;
            write_key (uris.patch_add);
            write_object (addFrame, id, uris.atom_Object);
            write_key (property);
            write_primitive (value);
            pop_frame (addFrame);

            ForgeFrame removeFrame;
            write_key (uris.patch_remove);
            write_object (removeFrame, id, uris.atom_Object);
            pop_frame (removeFrame);

            pop_frame (patchFrame);
            return patch;
        }

        ForgeRef patch_set (uint32_t prop, const LV2_Atom* value,
                            const LV2_Atom* subject = nullptr,
                            uint32_t        id      = 0,
                            uint32_t        ctx     = 0)
        {
            ForgeFrame frame;
            ForgeRef set (lv2_atom_forge_object (this, &frame, id, uris.patch_Set));

            if (subject)
            {
                lv2_atom_forge_key (this, uris.patch_subject);
                lv2_atom_forge_primitive (this, subject);
            }

            lv2_atom_forge_key (this, uris.patch_property);
            lv2_atom_forge_urid (this, prop);

            lv2_atom_forge_key (this, uris.patch_value);
            lv2_atom_forge_primitive (this, value);

            pop_frame (frame);
            return set;
        }

        ForgeRef patch_set_float (uint32_t prop, float val, const LV2_Atom* subject = nullptr, uint32_t id=0, uint32_t ctx=0)
        {
            LV2_Atom_Float af = { { sizeof(float), this->Float }, val };
            return patch_set (prop, (LV2_Atom*) &af, subject, id, ctx);
        }

        ForgeRef patch_set_double (uint32_t prop, double val, const LV2_Atom* subject = nullptr, uint32_t id=0, uint32_t ctx=0)
        {
            LV2_Atom_Double af = { { sizeof(double), this->Double }, val };
            return patch_set (prop, (LV2_Atom*) &af, subject, id, ctx);
        }

        ForgeRef patch_set_int (uint32_t prop, int32_t val, const LV2_Atom* subject = nullptr, uint32_t id=0, uint32_t ctx=0)
        {
            LV2_Atom_Int value = { { sizeof(int32_t), this->Int }, val };
            return patch_set (prop, (LV2_Atom*) &value, subject, id, ctx);
        }

        ForgeRef patch_set_long (uint32_t prop, int64_t val, const LV2_Atom* subject = nullptr, uint32_t id=0, uint32_t ctx=0)
        {
            LV2_Atom_Long value = { { sizeof(int64_t), this->Long }, val };
            return patch_set (prop, (LV2_Atom*) &value, subject, id, ctx);
        }

        ForgeRef patch_set_bool (uint32_t prop, bool val, const LV2_Atom* subject = nullptr, uint32_t id=0, uint32_t ctx=0)
        {
            LV2_Atom_Bool boolean = { { sizeof(bool), this->Bool }, val };
            return patch_set (prop, (const LV2_Atom*)&boolean, subject, id, ctx);
        }

        ForgeRef patch_set_string (uint32_t prop, const char* str, uint32_t str_type = 0, const LV2_Atom* subject = 0)
        {
            if (! str_type)
                str_type = this->String;

            ForgeFrame frame;
            ForgeRef set (lv2_atom_forge_object (this, &frame, 0, uris.patch_Set));

            if (subject)
            {
                lv2_atom_forge_key (this, uris.patch_subject);
                lv2_atom_forge_primitive (this, subject);
            }

            lv2_atom_forge_key (this, uris.patch_property);
            lv2_atom_forge_urid (this, prop);

            lv2_atom_forge_key (this, uris.patch_value);
            lv2_atom_forge_typed_string (this, str_type, str, strlen(str) + 1);

            lv2_atom_forge_pop (this, &frame);
            return set;
        }

        ForgeRef patch_set_path (uint32_t prop, const char* path, const LV2_Atom* subject = nullptr)
        {
            return patch_set_string (prop, path, this->Path, subject);
        }

        const URIs& uris;

    private:
        LV2_URID_Map* p_map;
        LV2_URID_Unmap* p_unmap;
    };

}  /* namespace KSP1 */

#endif  /* KSP1_FORGE_H */
