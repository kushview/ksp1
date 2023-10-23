/*
    LV2Plugin.h - This file is part of KSP1
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

#ifndef KSP1_LV2_PLUGIN_H
#define KSP1_LV2_PLUGIN_H

#include "../../libs/lvtk/lvtk/plugin.hpp"
#include "Forge.h"
#include "KSP1.h"

namespace KSP1 {

struct PatchMessage {
    PatchMessage (const URIs& u, const lvtk::AtomObject& o) : uris (u), object (o) {}
    const URIs& uris;
    const lvtk::AtomObject object;
    const lvtk::Atom subject;
};

struct PatchDelete : public PatchMessage {
    PatchDelete (const URIs& u, const lvtk::AtomObject& o)
        : PatchMessage (u, o) {
        lv2_atom_object_get (o, uris.patch_subject, &subject, 0);
    }
};

struct PatchGet : public PatchMessage {
    PatchGet (const URIs& u, const lvtk::AtomObject& o)
        : PatchMessage (u, o) {
        lv2_atom_object_get (o,
                             uris.patch_subject,
                             &subject,
                             0);
    }
};

struct PatchSet : public PatchMessage {
    inline PatchSet (const URIs& uris, const lvtk::AtomObject& obj)
        : PatchMessage (uris, obj) {
        lv2_atom_object_get (obj,
                             uris.patch_subject,
                             &subject,
                             uris.patch_property,
                             &property,
                             uris.patch_value,
                             &value,
                             0);
    }

    const lvtk::Atom property, value;
};

struct Patch : public PatchMessage {
    Patch (const URIs& uris, const lvtk::AtomObject& obj)
        : PatchMessage (uris, obj) {
        lv2_atom_object_get (obj,
                             uris.patch_subject,
                             &subject,
                             uris.patch_add,
                             &add,
                             uris.patch_remove,
                             &remove,
                             0);
    }

    const lvtk::Atom add, remove;
};

struct PatchResponse {
    PatchResponse (const URIs& uris, const AtomObject& o)
        : object (o) {
        assert (object.otype() == uris.patch_Response);
        lv2_atom_object_get (object,
                             uris.patch_body,
                             &body,
                             uris.patch_request,
                             &request,
                             uris.patch_sequenceNumber,
                             &sequenceNumber,
                             0);
    }

    const AtomObject object;
    const Atom body, request, sequenceNumber;
};

class JobManager;
class LV2Plugin;
class SampleCache;
class SamplerSynth;
typedef lvtk::Plugin<LV2Plugin, lvtk::URID<true>, lvtk::Worker<true>,
                     lvtk::State<false>>
    LV2PluginType;

class LV2Plugin : public LV2PluginType {
public:
    LV2Plugin (double _sampleRate);
    ~LV2Plugin();

    void activate();
    void connect_port (uint32_t port, void* ptr);
    void deactivate();
    void run (uint32_t nframes);

    const URIs& get_uris() const { return *uris; }
    SampleCache& get_sample_cache();
    SamplerSynth* get_sampler_synth() const;
    void trigger_restored();

    // LV2 Worker Callbacks
    lvtk::WorkerStatus work (lvtk::WorkerRespond& respond, uint32_t size, const void* data);
    lvtk::WorkerStatus work_response (uint32_t size, const void* body);
    lvtk::WorkerStatus end_run();

    // LV2 State Callbacks
    lvtk::StateStatus save (lvtk::StateStore& store, uint32_t flags, const lvtk::FeatureVec& features);
    lvtk::StateStatus restore (lvtk::StateRetrieve& retrieve, uint32_t flags, const lvtk::FeatureVec& features);

private:
    double sampleRate;
    float lastGain;
    ForgeFrame notifyFrame;
    LV2_Atom_Sequence* notifyPort;
    ScopedPointer<SamplerSynth> retainer;
    ScopedPointer<Forge> forge;
    ScopedPointer<SamplerSynth> sampler;
    ScopedPointer<KSP1::URIs> uris;
    ScopedPointer<JobManager> jobs;
    int32_t procFrame;
    float* audioIns[2];
    float* audioOuts[2];
    MidiBuffer midiIn;
    AtomicValue<int> wasRestored;

    void handle_patch_get (const PatchGet& obj);
    void handle_patch_set (const PatchSet& obj);
    void handle_patch (const Patch& patch);

    void handle_patch_set_root_level (const PatchSet& set);
};

} /* namespace KSP1 */

#endif // KSP1_LV2_PLUGIN_H
