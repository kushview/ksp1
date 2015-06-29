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

#ifndef KSP1_URIS_H
#define KSP1_URIS_H

#include <functional>
#include <string>
#include <stdint.h>

#include <lv2/lv2plug.in/ns/ext/atom/atom.h>
#include <lv2/lv2plug.in/ns/ext/urid/urid.h>
#include <lv2/lv2plug.in/ns/ext/midi/midi.h>
#include <lv2/lv2plug.in/ns/ext/patch/patch.h>
#include <lv2/lv2plug.in/ns/ext/parameters/parameters.h>

#define KSP1_URI                    "http://kushview.net/ns/ksp1"
#define NS_KSP1                     KSP1_URI "#"

#define KSP1_GUI_URI                "http://kushview.net/ns/ksp1/gui"
#define NS_KSP1_GUI                 KSP1_GUI_URI "#"

#define KSP1__Instrument            NS_KSP1 "Instrument"
#define KSP1__Key                   NS_KSP1 "Key"
#define KSP1__Layer                 NS_KSP1 "Layer"
#define KSP1__LayerData             NS_KSP1 "LayerData"
#define KSP1__SamplerSynth          NS_KSP1 "SamplerSynth"

#define KSP1_ASSETS_URI             NS_KSP1 "/assets"
#define NS_ASSETS                   KSP1_ASSETS_URI "#"
#define KSP1_ASSETS__Search         NS_ASSETS "Search"

#define KSP1_JOBS_URI               "http://kushview.net/ns/jobs"
#define NS_JOBS                     KSP1_JOBS_URI "#"
#define KSP1_JOBS__disposeObject    NS_JOBS "freeObject"
#define KSP1_JOBS__installObject    NS_JOBS "installObject"
#define KSP1_JOBS__finalizeJob      NS_JOBS "finalizeJob"
#define KSP1_JOBS__Disposable       NS_JOBS "Disposable"
#define KSP1_JOBS__ObjectRef        NS_JOBS "ObjectRef"

#define KSP1_SLUGS_URI              "http://kushview.net/ns/slugs"
#define NS_SLUGS                    KSP1_SLUGS_URI "#"
#define KSP1_SLUGS__key             NS_SLUGS "key"
#define KSP1_SLUGS__length          NS_SLUGS "length"
#define KSP1_SLUGS__object          NS_SLUGS "object"
#define KSP1_SLUGS__volume          NS_SLUGS "volume"
#define KSP1_SLUGS__file            NS_SLUGS "file"
#define KSP1_SLUGS__type            NS_SLUGS "type"
#define KSP1_SLUGS__note            NS_SLUGS "note"
#define KSP1_SLUGS__pitch           NS_SLUGS "pitch"
#define KSP1_SLUGS__panning         NS_SLUGS "panning"
#define KSP1_SLUGS__velocityUpper   NS_SLUGS "velocityUpper"
#define KSP1_SLUGS__velocityLower   NS_SLUGS "velocityLower"
#define KSP1_SLUGS__triggerMode     NS_SLUGS "triggerMode"
#define KSP1_SLUGS__voiceGroup      NS_SLUGS "voiceGroup"
#define KSP1_SLUGS__layer           NS_SLUGS "layer"
#define KSP1_SLUGS__start           NS_SLUGS "start"
#define KSP1_SLUGS__index           NS_SLUGS "index"
#define KSP1_SLUGS__offset          NS_SLUGS "offset"
#define KSP1_SLUGS__name            NS_SLUGS "name"
#define KSP1_SLUGS__cutoff          NS_SLUGS "cutoff"
#define KSP1_SLUGS__resonance       NS_SLUGS "resonance"

namespace KSP1
{
    struct URIs
    {
        typedef std::function<LV2_URID(const char*)> MapFunc;

        URIs (MapFunc &map)
            : assets_Search       (map (KSP1_ASSETS__Search)),
        
              atom_eventTransfer  (map (LV2_ATOM__eventTransfer)),
              atom_Object         (map (LV2_ATOM__Object)),
              atom_Path           (map (LV2_ATOM__Path)),
              atom_URID           (map (LV2_ATOM__URID)),
        
              ksp1_Instrument     (map (KSP1__Instrument)),
              ksp1_Key            (map (KSP1__Key)),
              ksp1_Layer          (map (KSP1__Layer)),
              ksp1_LayerData      (map (KSP1__LayerData)),
              ksp1_SamplerSound   (map (NS_KSP1 "SamplerSound")),
              ksp1_SamplerSynth   (map (KSP1__SamplerSynth)),
              ksp1_SqlQuery       (map (NS_KSP1 "SqlQuery")),
        
              jobs_disposeObject  (map (KSP1_JOBS__disposeObject)),
              jobs_finalizeJob    (map (KSP1_JOBS__finalizeJob)),
              jobs_installObject  (map (KSP1_JOBS__installObject)),
              jobs_Disposable     (map (KSP1_JOBS__Disposable)),
              jobs_ObjectRef      (map (KSP1_JOBS__ObjectRef)),
        
              slugs_cutoff          (map (KSP1_SLUGS__cutoff)),
              slugs_file            (map (KSP1_SLUGS__file)),
              slugs_index           (map (KSP1_SLUGS__index)),
              slugs_key             (map (KSP1_SLUGS__key)),
              slugs_layer           (map (KSP1_SLUGS__layer)),
              slugs_length          (map (KSP1_SLUGS__length)),
              slugs_name            (map (KSP1_SLUGS__name)),
              slugs_note            (map (KSP1_SLUGS__note)),
              slugs_object          (map (KSP1_SLUGS__object)),
              slugs_offset          (map (KSP1_SLUGS__offset)),
              slugs_panning         (map (KSP1_SLUGS__panning)),
              slugs_parent          (map (NS_SLUGS "parent")),
              slugs_pitch           (map (KSP1_SLUGS__pitch)),
              slugs_resonance       (map (KSP1_SLUGS__resonance)),
              slugs_start           (map (KSP1_SLUGS__start)),
              slugs_sql             (map (NS_SLUGS "sql")),
              slugs_triggerMode     (map (KSP1_SLUGS__triggerMode)),
              slugs_type            (map (KSP1_SLUGS__type)),
              slugs_velocityUpper   (map (KSP1_SLUGS__velocityUpper)),
              slugs_velocityLower   (map (KSP1_SLUGS__velocityLower)),
              slugs_voiceGroup      (map (KSP1_SLUGS__voiceGroup)),
              slugs_volume          (map (KSP1_SLUGS__volume)),
        
              midi_MidiEvent        (map (LV2_MIDI__MidiEvent)),
        
              patch_add             (map (LV2_PATCH__add)),
              patch_body            (map (LV2_PATCH__body)),
              patch_property        (map (LV2_PATCH__property)),
              patch_remove          (map (LV2_PATCH__remove)),
              patch_request         (map (LV2_PATCH__request)),
              patch_sequenceNumber  (map (LV2_PATCH__sequenceNumber)),
              patch_subject         (map (LV2_PATCH__subject)),
              patch_value           (map (LV2_PATCH__value)),
              patch_Delete          (map (LV2_PATCH__Delete)),
              patch_Get             (map (LV2_PATCH__Get)),
              patch_Set             (map (LV2_PATCH__Set)),
              patch_Patch           (map (LV2_PATCH__Patch)),
              patch_Post            (map (LV2_PATCH__Post)),
              patch_Put             (map (LV2_PATCH__Put)),
              patch_Response        (map (LV2_PATCH__Response)),
              patch_Ack             (map (LV2_PATCH__Ack))
        { }

        const uint32_t assets_Search;
        
        const uint32_t atom_eventTransfer;
        const uint32_t atom_Object;
        const uint32_t atom_Path;
        const uint32_t atom_URID;

        const uint32_t ksp1_Instrument;
        const uint32_t ksp1_Key;
        const uint32_t ksp1_Layer;
        const uint32_t ksp1_LayerData;
        const uint32_t ksp1_SamplerSound;
        const uint32_t ksp1_SamplerSynth;
        const uint32_t ksp1_SqlQuery;

        const uint32_t jobs_disposeObject;
        const uint32_t jobs_finalizeJob;
        const uint32_t jobs_installObject;
        const uint32_t jobs_Disposable;
        const uint32_t jobs_ObjectRef;

        const uint32_t slugs_cutoff;
        const uint32_t slugs_file;
        const uint32_t slugs_index;
        const uint32_t slugs_key;
        const uint32_t slugs_layer;
        const uint32_t slugs_length;
        const uint32_t slugs_name;
        const uint32_t slugs_note;
        const uint32_t slugs_object;
        const uint32_t slugs_offset;
        const uint32_t slugs_panning;
        const uint32_t slugs_parent;
        const uint32_t slugs_pitch;
        const uint32_t slugs_resonance;
        const uint32_t slugs_start;
        const uint32_t slugs_sql;
        const uint32_t slugs_triggerMode;
        const uint32_t slugs_type;
        const uint32_t slugs_velocityUpper;
        const uint32_t slugs_velocityLower;
        const uint32_t slugs_voiceGroup;
        const uint32_t slugs_volume;
        
        const uint32_t midi_MidiEvent;

        const uint32_t patch_add;
        const uint32_t patch_body;
        const uint32_t patch_property;
        const uint32_t patch_remove;
        const uint32_t patch_request;
        const uint32_t patch_sequenceNumber;
        const uint32_t patch_subject;
        const uint32_t patch_value;
        const uint32_t patch_Delete;
        const uint32_t patch_Get;
        const uint32_t patch_Set;
        const uint32_t patch_Patch;
        const uint32_t patch_Post;
        const uint32_t patch_Put;
        const uint32_t patch_Response;
        const uint32_t patch_Ack;

        inline static std::string slugToURI (const char* slug) {
            static const std::string base = NS_SLUGS;
            return base + std::string (slug);
        }

        template<class MAP>
        inline static LV2_URID slugToURID (MAP& map, const char* slug) {
            return map.map (slugToURI (slug).c_str());
        }

        URIs(const URIs&);
        URIs& operator= (const URIs&);
    };
}

#endif /* KSP1_URIS_H */
