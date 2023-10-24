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

#include <lvtk/plugin.hpp>

#include <lvtk/ext/atom.hpp>
#include <lvtk/ext/state.hpp>
#include <lvtk/ext/urid.hpp>
#include <lvtk/ext/worker.hpp>

#include "urids.hpp"

namespace ksp1 {

struct PatchMessage {
    PatchMessage (const URIs& u, const lvtk::Object& o) : uris (u), object (o) {}
    const URIs& uris;
    const lvtk::Object object;
    const lvtk::Atom subject;
};

struct PatchDelete : public PatchMessage {
    PatchDelete (const URIs& u, const lvtk::Object& o)
        : PatchMessage (u, o) {
        lv2_atom_object_get (o, uris.patch_subject, &subject, 0);
    }
};

struct PatchGet : public PatchMessage {
    PatchGet (const URIs& u, const lvtk::Object& o)
        : PatchMessage (u, o) {
        lv2_atom_object_get (o,
                             uris.patch_subject,
                             &subject,
                             0);
    }
};

struct PatchSet : public PatchMessage {
    inline PatchSet (const URIs& uris, const lvtk::Object& obj)
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
    Patch (const URIs& uris, const lvtk::Object& obj)
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
    PatchResponse (const URIs& uris, const lvtk::Object& o)
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

    const lvtk::Object object;
    const lvtk::Atom body, request, sequenceNumber;
};

class JobManager;
class LV2Plugin;
class SampleCache;
class SamplerSynth;

using PluginBase = lvtk::Plugin<LV2Plugin, lvtk::URID, lvtk::Worker, lvtk::State>;

class LV2Plugin : public PluginBase {
public:
    explicit LV2Plugin (const lvtk::Args&);
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
    lvtk::StateStatus save (lvtk::StateStore& store, uint32_t flags, const lvtk::FeatureList& features);
    lvtk::StateStatus restore (lvtk::StateRetrieve& retrieve, uint32_t flags, const lvtk::FeatureList& features);

private:
    double sampleRate;
    float lastGain;
    lvtk::ForgeFrame notifyFrame;
    LV2_Atom_Sequence* atomIn  = nullptr;
    LV2_Atom_Sequence* atomOut = nullptr;
    std::unique_ptr<SamplerSynth> retainer;
    lvtk::Forge forge;
    std::unique_ptr<SamplerSynth> sampler;
    std::unique_ptr<ksp1::URIs> uris;
    std::unique_ptr<JobManager> jobs;
    int32_t procFrame;
    float* audioIns[2];
    float* audioOuts[2];
    juce::MidiBuffer midiIn;
    std::atomic<int> wasRestored;

    void handle_patch_get (const PatchGet& obj);
    void handle_patch_set (const PatchSet& obj);
    void handle_patch (const Patch& patch);

    void handle_patch_set_root_level (const PatchSet& set);
};

} // namespace ksp1
