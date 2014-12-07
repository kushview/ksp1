/*
    LV2Plugin.cpp - This file is part of KSP1
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

#include "engine/Jobs.h"
#include "engine/LayerData.h"
#include "engine/SampleCache.h"
#include "engine/SamplerSounds.h"
#include "engine/SamplerSynth.h"
#include "Ports.h"
#include "URIs.h"
#include "LV2Plugin.h"

using namespace lvtk;

namespace KSP1 {



class LoadInstrumentJob : public Job {
public:

};


static Array<LV2Plugin*> sPlugins;

LV2Plugin::LV2Plugin (double _sampleRate)
    : LV2PluginType (2),
      sampleRate (_sampleRate), lastGain (1.0f)
{
    sampler = SamplerSynth::create();
    retainer = SamplerSynth::create();

    var json;
    sampler->getNestedVariant (json);

    URIs::MapFunc mf = std::bind (&LV2Plugin::map, this, std::placeholders::_1);
    uris  = new URIs (mf);
    forge = new Forge (*uris, get_urid_map(), get_urid_unmap());
    jobs  = new JobManager (*this);

    sPlugins.add (this);
}

LV2Plugin::~LV2Plugin()
{
    sPlugins.removeFirstMatchingValue (this);
    jobs = nullptr;
    sampler = nullptr;
    forge = nullptr;
    uris = nullptr;
}

void LV2Plugin::activate()
{
    lastGain = 1.0f;
    sampler->setCurrentPlaybackSampleRate (sampleRate);
}

void LV2Plugin::connect_port (uint32_t port, void *ptr)
{
    switch (port)
    {
        case Port::MainLeft:
            audioOuts[0] = static_cast<float*> (ptr);
            break;
        case Port::MainRight:
            audioOuts[1] = static_cast<float*> (ptr);
            break;
        case Port::AtomOutput:
            notifyPort = static_cast<LV2_Atom_Sequence*> (ptr);
        default:
            LV2PluginType::connect_port (port, ptr);
            break;
    }
}

void LV2Plugin::deactivate()
{

}

void LV2Plugin::run (uint32_t nframes)
{
    const lvtk::AtomSequence seq (p<LV2_Atom_Sequence*> (Port::AtomInput));
    forge->set_buffer ((uint8_t*)notifyPort, notifyPort->atom.size);
    forge->sequence_head (notifyFrame, 0);

    MidiBuffer buf;
    AudioSampleBuffer audio (audioOuts, 2, nframes);
    audio.clear();

    for (const lvtk::AtomEvent& ev : seq)
    {
        if (ev.body.type == uris->midi_MidiEvent)
        {
            buf.addEvent (LV2_ATOM_BODY (&ev.body), ev.body.size,
                    (ev.time.frames < nframes) ? ev.time.frames : nframes - 1);
        }
        else if (ev.body.type == uris->atom_Object)
        {
            const lvtk::AtomObject obj (&ev.body);

            if (obj.otype() == uris->patch_Set) {
                PatchSet set (*uris, obj);
                handle_patch_set (set);
            }
            else if (obj.otype() == uris->patch_Get) {
                PatchGet get (*uris, obj);
                handle_patch_get (get);
            }
            else if (obj.otype() == uris->patch_Patch)
            {
                Patch patch (*uris, obj);
                handle_patch (patch);
            }
            else if (obj.otype() == uris->patch_Delete)
            {
                const PatchDelete del (*uris, obj);
                if (del.subject.has_object_type (uris->ksp1_Layer)) {
                    lvtk::AtomObject layer (del.subject.as_object());
                    if (LayerData* data = sampler->getLayerData (*uris, layer))
                        sampler->recycleLayerData (data);
                }
            }
        }

        else
            DBG (unmap (ev.body.type));
    }

    sampler->renderNextBlock (audio, buf, 0, static_cast<int> (nframes));
    audio.applyGainRamp (0, nframes, lastGain, sampler->getMasterGain());
    lastGain = sampler->getMasterGain();
}



WorkerStatus LV2Plugin::end_run()
{
    forge->pop (notifyFrame);
    return WORKER_SUCCESS;
}

void LV2Plugin::handle_patch_get (const PatchGet &obj)
{
    if (! obj.subject) {
        DBG ("patch get with null subject");
        return;
    }

    if (obj.subject.has_object_type (uris->ksp1_Key))
    {
        if (SamplerSound* sound = sampler->getSound (*uris, obj.subject.as_object()))
        {
            forge->frame_time (0);
            sound->writeAtomObject (*forge);
            for (LayerData* layer : *sound) {
                forge->frame_time (0);
                layer->writeAtomObject (*forge);
            }
        }
    }
    else if (obj.subject.type() == uris->atom_URID) {

    }
}

void LV2Plugin::do_patch_set_root_level (const PatchSet& set) {
   /* DBG ("set_root_level");
    DBG ("  " << unmap (set.property.as_urid())); */
}

void LV2Plugin::handle_patch_set (const PatchSet& set)
{
    if (! set.subject)
        return do_patch_set_root_level (set);

    if (set.subject.has_object_type (uris->ksp1_Layer)) {
        if (LayerData* data = sampler->getLayerData (*uris, set.subject.as_object())) {
            data->setProperty (*uris, set);
        } else {
            DBG ("can't set layer property: " << unmap (set.property.as_urid()));
        }
    }
    else if (set.subject.has_object_type (uris->ksp1_Key))
    {
        if (set.property.as_urid() == uris->slugs_note)
        {
            jassert (set.subject.as_object().id() != 0);
            if (SamplerSound* sound = sampler->moveSound (set.subject.as_object().id(), set.value.as_int()))
                { }
        }
        else
        {
            if (SamplerSound* snd = sampler->getSound (*uris, set.subject.as_object()))
                snd->setProperty (*uris, set);
        }
    }
    else if (set.subject.has_type_and_equals (forge->URID, uris->ksp1_Instrument))
    {
        if (set.property.as_urid() == uris->slugs_file &&
            set.value.type() == uris->atom_Path)
        {
            const Atom atom (set.object.cobj());
            schedule_work (atom.total_size(), atom.cobj());
        }
        else
        {
            sampler->setProperty (*uris, set);
        }
    }
}

void LV2Plugin::handle_patch (const Patch& patch)
{
    if (patch.subject.has_object_type (uris->ksp1_Key))
    {
        if (patch.add)
        {
            for (const auto& p : patch.add.as_object())
            {
                if (p.key == uris->slugs_layer)
                {
                    schedule_work (lv2_atom_total_size (&p.value), &p.value);
                }
            }
        }

        if (patch.remove) {

        }
    }

    if (patch.subject.has_type_and_equals (uris->atom_URID, uris->ksp1_Instrument))
    {
        if (patch.add)
        {
            for (const auto& p : patch.add.as_object())
            {
                if (p.key == uris->slugs_key)
                {
                    schedule_work (lv2_atom_total_size (&p.value), &p.value);
                }
            }
        }

        if (patch.remove) {

        }
    }
}

WorkerStatus LV2Plugin::work (WorkerRespond &respond, uint32_t size, const void* data)
{
    const lvtk::Atom atom (data);
    if (atom.total_size() != size) {
        DBG ("work: atom size mismatch: " << unmap (atom.type()));
        return WORKER_ERR_UNKNOWN;
    }

    return jobs->work (respond, *uris, atom);
}

WorkerStatus LV2Plugin::work_response (uint32_t size, const void* body)
{
    const lvtk::Atom atom (body);
    if (atom.total_size() != size) {
        DBG ("work_response: atom size mismatch: " << unmap (atom.type()));
        return WORKER_ERR_UNKNOWN;
    }

    /* The job manager (and jobs) will send these when
       jobs need finishing touches and deleted */
    if (atom.type() == uris->jobs_finalizeJob) {
        schedule_work (size, body);
        return WORKER_SUCCESS;
    }

    if (atom.type() == 100100) {
        forge->frame_time (0);
        lv2_atom_forge_primitive (forge, atom);
    }

    if (atom.has_object_type (uris->jobs_ObjectRef))
    {
        ObjectRef ref (*uris, atom.as_object());
        if (ref.has_class_type (uris->ksp1_SamplerSynth))
        {
            if (SamplerSynth* next = ref.get<SamplerSynth>())
            {
                if (SamplerSynth* old = sampler.release())
                    jobs->dispose (old);

                DBG ("LV2Plugin: installing new synth");
                sampler = next;
                sampler->setCurrentPlaybackSampleRate (sampleRate);
                forge->frame_time (0);
                ForgeFrame frame;
                forge->write_object (frame, 1, uris->ksp1_SamplerSynth);
                forge->write_key (uris->slugs_index); forge->write_int (1);
                forge->pop_frame (frame);
            }
        }
        else if (ref.has_class_type (uris->ksp1_Key))
        {
            if (SamplerSound* sound = ref.get<SamplerSound>())
            {
                if (sampler->insertSound (sound))
                {
                    forge->frame_time(0);
                    sound->writeAtomObject (*forge);
                }
                else
                {
                    DBG ("Failed inserted new sound on note: " << sound->getRootNote());
                }
            }
        }
        else if (ref.has_class_type (uris->ksp1_Layer))
        {
            if (LayerData* data = ref.get<LayerData>())
            {
                if (sampler->insertLayerData (data->getParent(), data))
                {
                    forge->frame_time (0);
                    data->writeAtomObject (*forge);
                }
                else
                {
                    DBG ("Failed inserting new layer");
                    data->reset();
                }
            }
        }
    }

    return WORKER_SUCCESS;
}

StateStatus LV2Plugin::save (StateStore &store, uint32_t /*flags*/, const FeatureVec &features)
{
    LV2_State_Make_Path* make_path = nullptr;
    LV2_State_Map_Path* map_path = nullptr;

    for (const LV2_Feature* f : features)
    {
        if (0 == strcmp (LV2_STATE__makePath, f->URI)) {
            make_path = reinterpret_cast<LV2_State_Make_Path*> (f->data);
        }

        if (0 == strcmp (LV2_STATE__mapPath, f->URI)) {
            map_path = reinterpret_cast<LV2_State_Map_Path*> (f->data);
        }
    }

    var json;
    if (sampler->getNestedVariant (json))
    {
        if (map_path && make_path)
        {
            if (char* absolute = make_path->path (make_path->handle, "data.json")) {
                if (char* abstract = map_path->abstract_path (map_path->handle, absolute)) {
                    const File file (String::fromUTF8 (absolute));
                    FileOutputStream fos (file);
                    JSON::writeToStream (fos, json);
                    store (uris->slugs_file, abstract, strlen (abstract) + 1, forge->Path, STATE_IS_POD);
                    std::free (abstract);
                }
                std::free (absolute);
            }
        }
        else
        {
            const String data = JSON::toString (json, true);
            store (uris->ksp1_SamplerSynth, data.toRawUTF8(), data.length() + 1, forge->String, STATE_IS_POD);
        }
    }

    return STATE_SUCCESS;
}

StateStatus LV2Plugin::restore (StateRetrieve &retrieve, uint32_t flags, const FeatureVec &features)
{
    LV2_State_Make_Path* make_path = nullptr;
    LV2_State_Map_Path* map_path = nullptr;

    for (const LV2_Feature* f : features)
    {
        if (0 == strcmp (LV2_STATE__makePath, f->URI)) {
            make_path = reinterpret_cast<LV2_State_Make_Path*> (f->data);
        }

        if (0 == strcmp (LV2_STATE__mapPath, f->URI)) {
            map_path = reinterpret_cast<LV2_State_Map_Path*> (f->data);
        }
    }
    if (! map_path)
        return lvtk::STATE_ERR_UNKNOWN;

    size_t size; uint32_t type; uint32_t rflags;
    if (const void* data = retrieve (uris->slugs_file, &size, &type, &rflags))
    {
        if (type == uris->atom_Path)
        {
            const char* abstract = (const char*) data;
            if (char* absolute = map_path->absolute_path (map_path->handle, abstract))
            {
                const File file (String::fromUTF8 (absolute));
                sampler->loadFile (file);
            }
        }
    }
    else if (const void* data = retrieve (uris->ksp1_SamplerSynth, &size, &type, &rflags))
    {
        const String jsonStr (String::fromUTF8 ((const char*) data));
        const var json = JSON::parse (jsonStr);

        DBG (JSON::toString(json));
    }

    return STATE_SUCCESS;
}

SampleCache& LV2Plugin::get_sample_cache()
{
    return retainer->getSampleCache();
}

}

static unsigned ksp1i = KSP1::LV2Plugin::register_class (KSP1_URI);

#include "../../libs/lvtk/src/plugin.cpp"
