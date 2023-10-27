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

#include <lv2/core/lv2_util.h>

#include "jobs.hpp"

#include "cache.hpp"
#include "layerdata.hpp"
#include "plugin.hpp"
#include "ports.hpp"
#include "sounds.hpp"
#include "synth.hpp"

namespace ksp1 {

template <typename T>
using Array = juce::Array<T>;
using var   = juce::var;

class LoadInstrumentJob : public Job {
public:
};

LV2Plugin::LV2Plugin (const lvtk::Args& args)
    : PluginBase (args),
      _bundlePath (args.bundle),
      sampleRate (args.sample_rate),
      lastGain (1.0f),
      wasRestored (0) {
    sampler.reset (SamplerSynth::create());
    retainer.reset (SamplerSynth::create());

    auto& c = sampler->getSampleCache();
    c.addSearchPath (juce::File (_bundlePath));

    for (const auto& f : args.features) {
        if (std::strcmp (f.URI, LV2_URID__map) == 0)
            lv2_atom_forge_init (&forge, (LV2_URID_Map*) f.data);
    }

    URIs::MapFunc mf = std::bind (&LV2Plugin::map_uri, this, std::placeholders::_1);
    uris             = std::make_unique<URIs> (mf);
    jobs             = std::make_unique<JobManager> (*this);
}

LV2Plugin::~LV2Plugin() {
    jobs     = nullptr;
    retainer = nullptr;
    sampler  = nullptr;
    uris     = nullptr;
}

void LV2Plugin::activate() {
    lastGain = 1.0f;
    sampler->setCurrentPlaybackSampleRate (sampleRate);
    midiIn.ensureSize (2048);

    if (true) {
#if 1
        juce::Array<juce::File> paths = {
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-04/R-8808 conga.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-04/R-8808 cowbell.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-04/R-8808 maracas.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-04/R-8flange k.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-04/R-8elec ride bell.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-04/R-8high q.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-04/R-8808 k.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-04/R-8808s stick.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-04/R-8808 clave.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-04/R-8elec crash.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-04/R-8slap2.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-04/R-8elec k.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-04/R-8elec s.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-04/R-8elec tom.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-04/R-8808 hh c.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-04/R-8flange s.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-04/R-8rap noise.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-04/R-8808 hh o.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-04/R-8808 tom.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-04/R-8synth bass.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-04/R-8808 ride.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-04/R-8reverb s2.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-04/R-8808 s.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-04/R-8cntrl k.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-04/R-8wood box.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-04/R-8bend tom.wav"),
        };
#else
        juce::Array<juce::File> paths = {
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-11/R8 Bell CYM 2.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-11/R8 Bowl.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-11/R8 Can 2.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-11/R8 CHH 2.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-11/R8 Chime.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-11/R8 Bell CYM 2.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-11/R8 Crash CYM 7.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-11/R8 Short Chinese CYM.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-11/R8 Crash CYM 8.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-11/R8 OHH 3.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-11/R8 CHH 2.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-11/R8 Crash CYM 6.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-11/R8 Ride Bell CYM 2.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-11/R8 Cowbell 3.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-11/R8 Spoons.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-11/R8 Pedal CHH 2.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-11/R8 Slay Bell.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-11/R8 Bowl.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-11/R8 OHH 2.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-11/R8 Can 2.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-11/R8 RNB CRS.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-11/R8 Pedal CHH 3.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-11/R8 Hand CYM 1.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-11/R8 Splash CYM 3.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-11/R8 Flex Tone.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-11/R8 Ride CYM 2.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-11/R8 Steel Lite Drum.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-11/R8 Mute Maracas.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-11/R8 CHH 3.wav"),
            juce::File (_bundlePath).getChildFile ("kits/sn-r8-11/R8 Mute Castanets.wav")
        };
#endif

        int startNote = 54;
        for (const auto& file : paths) {
            auto data = sampler->getSampleCache().getLayerData (true);
            data->loadAudioFile (file);
            auto sound = new SamplerSound (startNote, 1);
            // sound->setRootNote (0);
            // sound->setKeyLength (127);
            // sound->setMidiChannel (1);
            if (sound->insertLayerData (data)) {
                sampler->insertSound (sound);
                sound->setDefaultLength();
                startNote++;
            } else {
                std::clog << "could not load: " << file.getFullPathName() << std::endl;
                delete data;
                delete sound;
            }
        }

        auto obj       = sampler->createDynamicObject();
        juce::var json = obj.get();
        std::clog << juce::JSON::toString (json, false) << std::endl;

    } else {
        std::clog << "couldn't load test buffer\n";
    }
}

void LV2Plugin::connect_port (uint32_t port, void* ptr) {
    switch (port) {
        case Port::MainLeft:
            audioOuts[0] = static_cast<float*> (ptr);
            break;
        case Port::MainRight:
            audioOuts[1] = static_cast<float*> (ptr);
            break;
        case Port::AtomInput:
            atomIn = static_cast<LV2_Atom_Sequence*> (ptr);
            break;
        case Port::AtomOutput:
            atomOut = static_cast<LV2_Atom_Sequence*> (ptr);
            break;
    }
}

void LV2Plugin::deactivate() {
}

void LV2Plugin::run (uint32_t nframes) {
    const int numSamples = static_cast<int> (nframes);
    lv2_atom_forge_set_buffer (&forge, (uint8_t*) atomOut, ((LV2_Atom*) atomOut)->size);
    lv2_atom_forge_sequence_head (&forge, &notifyFrame, 0);

    const lvtk::Sequence seq (atomIn);

#if 0
    forge->set_buffer ((uint8_t*) notifyPort, notifyPort->atom.size);
    forge->sequence_head (notifyFrame, 0);

    if (wasRestored.load() != 0) {
        if (wasRestored.store (0)) {
            forge->frame_time (0);
            ForgeFrame frame;
            forge->write_object (frame, 1, uris->ksp1_SamplerSynth);
            forge->write_key (uris->slugs_index);
            forge->write_int (1);
            forge->pop_frame (frame);
        }
    }
#endif

    juce::AudioSampleBuffer audio (audioOuts, 2, numSamples);
    audio.clear();

    procFrame = 0;
    for (const lvtk::AtomEvent& ev : seq) {
        if (procFrame < ev.time.frames)
            sampler->renderNextBlock (audio, midiIn, procFrame, ev.time.frames - procFrame);

        if (ev.body.type == uris->midi_MidiEvent) {
            midiIn.addEvent (LV2_ATOM_BODY (&ev.body), ev.body.size, (ev.time.frames < numSamples) ? ev.time.frames : numSamples - 1);
        } else if (ev.body.type == uris->atom_Object) {
#if 0
            const lvtk::Object obj (&ev.body);

            if (obj.otype() == uris->patch_Set) {
                const PatchSet set (*uris, obj);
                handle_patch_set (set);
            } else if (obj.otype() == uris->patch_Get) {
                const PatchGet get (*uris, obj);
                handle_patch_get (get);
            } else if (obj.otype() == uris->patch_Patch) {
                const Patch patch (*uris, obj);
                handle_patch (patch);
            } else if (obj.otype() == uris->patch_Delete) {
                const PatchDelete del (*uris, obj);
                if (del.subject.has_object_type (uris->ksp1_Layer)) {
                    lvtk::AtomObject layer (del.subject.as_object());
                    if (LayerData* data = sampler->getLayerData (*uris, layer))
                        sampler->recycleLayerData (data);
                }
            }
#endif
        } else {
            DBG (unmap (ev.body.type));
        }

        procFrame = ev.time.frames;
    }

    if (procFrame < numSamples)
        sampler->renderNextBlock (audio, midiIn, procFrame, numSamples - procFrame);

    audio.applyGainRamp (0, numSamples, lastGain, sampler->getMasterGain());
    lastGain = sampler->getMasterGain();
    midiIn.clear();
}

void LV2Plugin::trigger_restored() {
    // while (! wasRestored.set (1)) {
    // }
}

lvtk::WorkerStatus LV2Plugin::end_run() {
    lv2_atom_forge_pop (&forge, &notifyFrame);
    return LV2_WORKER_SUCCESS;
}

SamplerSynth* LV2Plugin::get_sampler_synth() const { return sampler.get(); }

void LV2Plugin::handle_patch_get (const PatchGet& obj) {
    if (! obj.subject) {
        DBG ("patch get with null subject");
        return;
    }
#if 0
    if (obj.subject.type() == uris->atom_Object, uris->ksp1_Key) {
        if (SamplerSound* sound = sampler->getSound (*uris, obj.subject.as_object())) {
            forge->frame_time (0);
            sound->writeAtomObject (*forge);
            for (LayerData* layer : *sound) {
                forge->frame_time (0);
                layer->writeAtomObject (*forge);
            }
        }
    } else if (obj.subject.type() == uris->atom_URID) {
    }
#endif
}

void LV2Plugin::handle_patch_set_root_level (const PatchSet& set) {
    /* DBG ("set_root_level");
    DBG ("  " << unmap (set.property.as_urid())); */
}

void LV2Plugin::handle_patch_set (const PatchSet& set) {
    if (! set.subject)
        return handle_patch_set_root_level (set);

#if 0
    if (set.subject.has_object_type (uris->ksp1_Layer)) {
        if (LayerData* data = sampler->getLayerData (*uris, set.subject.as_object())) {
            data->setProperty (*uris, set);
            if (set.property.as_urid() == uris->slugs_length || set.property.as_urid() == uris->slugs_offset || set.property.as_urid() == uris->slugs_start) {
                if (SamplerSound* snd = data->getSound())
                    snd->setDefaultLength();
            }
        } else {
            //DBG ("can't set layer property: " << unmap (set.property.as_urid()));
        }
    } else if (set.subject.has_object_type (uris->ksp1_Key)) {
        if (set.property.as_urid() == uris->slugs_note) {
            jassert (set.subject.as_object().id() != 0);
            if (SamplerSound* sound = sampler->moveSound (set.subject.as_object().id(), set.value.as_int())) {
                (void) sound;
            }
        } else {
            if (SamplerSound* snd = sampler->getSound (*uris, set.subject.as_object()))
                snd->setProperty (*uris, set);
        }
    } else if (set.subject.has_type_and_equals (forge->URID, uris->ksp1_Instrument)) {
        if (set.property.as_urid() == uris->slugs_file && set.value.type() == uris->atom_Path) {
            const Atom atom (set.object.cobj());
            schedule_work (atom.total_size(), atom.cobj());
        } else {
            sampler->setProperty (*uris, set);
        }
    }
#endif
}

void LV2Plugin::handle_patch (const Patch& patch) {
#if 0
    if (patch.subject.has_object_type (uris->ksp1_Key)) {
        if (patch.add) {
            for (const auto& p : patch.add.as_object()) {
                if (p.key == uris->slugs_layer) {
                    schedule_work (lv2_atom_total_size (&p.value), &p.value);
                }
            }
        }

        if (patch.remove) {
        }
    }

    if (patch.subject.has_type_and_equals (uris->atom_URID, uris->ksp1_Instrument)) {
        if (patch.add) {
            for (const auto& p : patch.add.as_object()) {
                if (p.key == uris->slugs_key) {
                    schedule_work (lv2_atom_total_size (&p.value), &p.value);
                }
            }
        }

        if (patch.remove) {
        }
    }
#endif
}

lvtk::WorkerStatus LV2Plugin::work (lvtk::WorkerRespond& respond, uint32_t size, const void* data) {
    const lvtk::Atom atom (data);
    if (atom.total_size() != size) {
        DBG ("work: atom size mismatch: " << unmap (atom.type()));
        return LV2_WORKER_ERR_UNKNOWN;
    }

    return jobs->work (respond, *uris, atom);
}

lvtk::WorkerStatus LV2Plugin::work_response (uint32_t size, const void* body) {
    const lvtk::Atom atom (body);

    if (atom.total_size() != size) {
        DBG ("work_response: atom size mismatch: " << unmap (atom.type()));
        return LV2_WORKER_ERR_UNKNOWN;
    }

#if 0
    /* The job manager (and jobs) will send these when
       jobs need finishing touches and deleted */
    if (atom.type() == uris->jobs_finalizeJob) {
        schedule_work (size, body);
        return LV2_WORKER_SUCCESS;
    }

    if (atom.type() == 100100) {
        forge->frame_time (0);
        lv2_atom_forge_primitive (forge, atom);
    }

    if (atom.has_object_type (uris->jobs_ObjectRef)) {
        ObjectRef ref (*uris, atom.as_object());
        if (ref.has_class_type (uris->ksp1_SamplerSynth)) {
            if (SamplerSynth* next = ref.get<SamplerSynth>()) {
                if (SamplerSynth* old = sampler.release())
                    jobs->dispose (old);

                sampler = next;
                sampler->setCurrentPlaybackSampleRate (sampleRate);
                forge->frame_time (0);
                ForgeFrame frame;
                forge->write_object (frame, 1, uris->ksp1_SamplerSynth);
                forge->write_key (uris->slugs_index);
                forge->write_int (1);
                forge->pop_frame (frame);
            }
        } else if (ref.has_class_type (uris->ksp1_Key)) {
            if (SamplerSound* sound = ref.get<SamplerSound>()) {
                if (sampler->insertSound (sound)) {
                    forge->frame_time (0);
                    sound->writeAtomObject (*forge);
                } else {
                    DBG ("Failed inserted new sound on note: " << sound->getRootNote());
                }
            }
        } else if (ref.has_class_type (uris->ksp1_Layer)) {
            if (LayerData* data = ref.get<LayerData>()) {
                if (sampler->insertLayerData (data->getParent(), data)) {
                    forge->frame_time (0);
                    data->writeAtomObject (*forge);
                } else {
                    DBG ("Failed inserting new layer");
                    data->reset();
                }
            }
        }
    }
#endif

    return LV2_WORKER_SUCCESS;
}

lvtk::StateStatus LV2Plugin::save (lvtk::StateStore& store, uint32_t /*flags*/, const lvtk::FeatureList& features) {
    using juce::FileOutputStream;
    using juce::JSON;
    using juce::String;
    using juce::File;

    LV2_State_Make_Path* make_path = nullptr;
    LV2_State_Map_Path* map_path   = nullptr;

    for (const auto& f : features) {
        if (0 == strcmp (LV2_STATE__makePath, f.URI)) {
            make_path = reinterpret_cast<LV2_State_Make_Path*> (f.data);
        }

        if (0 == strcmp (LV2_STATE__mapPath, f.URI)) {
            map_path = reinterpret_cast<LV2_State_Map_Path*> (f.data);
        }
    }

#if 1

    if (auto obj = sampler->createDynamicObject()) {
        var json = obj.get();
        if (map_path && make_path) {
            if (char* absolute = make_path->path (make_path->handle, "data.json")) {
                if (char* abstract = map_path->abstract_path (map_path->handle, absolute)) {
                    const File file (String::fromUTF8 (absolute));
                    FileOutputStream fos (file);
                    JSON::writeToStream (fos, json);
                    store (uris->slugs_file, abstract, strlen (abstract) + 1, forge.Path, LV2_STATE_IS_POD);

                    std::free (abstract);
                }

                std::free (absolute);
            }
        } else {
            const String data = JSON::toString (json, true);
            store (uris->ksp1_SamplerSynth,
                   data.toRawUTF8(),
                   data.length() + 1,
                   forge.String,
                   LV2_STATE_IS_POD);
        }
    }
#else
    lvtk::ignore (map_path, make_path);
#endif

    return LV2_STATE_SUCCESS;
}

lvtk::StateStatus LV2Plugin::restore (lvtk::StateRetrieve& retrieve,
                                      uint32_t flags,
                                      const lvtk::FeatureList& features) {
#if 0
    LV2_State_Make_Path* make_path = nullptr;
    lvtk::ignore (make_path);
    LV2_State_Map_Path* map_path = nullptr;

    for (const auto& f : features) {
        if (0 == std::strcmp (LV2_STATE__makePath, f.URI)) {
            make_path = reinterpret_cast<LV2_State_Make_Path*> (f.data);
        }

        if (0 == std::strcmp (LV2_STATE__mapPath, f.URI)) {
            map_path = reinterpret_cast<LV2_State_Map_Path*> (f.data);
        }
    }

    if (! map_path)
        return LV2_STATE_ERR_UNKNOWN;

    size_t size;
    uint32_t type;
    uint32_t rflags;
    if (const void* data = retrieve (uris->slugs_file, &size, &type, &rflags)) {
        if (type == uris->atom_Path) {
            const char* abstract = (const char*) data;
            if (char* absolute = map_path->absolute_path (map_path->handle, abstract)) {
                const juce::File file (juce::String::fromUTF8 (absolute));
                sampler->loadFile (file);
            }
        }
    } else if (const void* data = retrieve (uris->ksp1_SamplerSynth, &size, &type, &rflags)) {
        const juce::String jsonStr (juce::String::fromUTF8 ((const char*) data));
        sampler->loadJSON (jsonStr);
    }

    trigger_restored();
#endif
    return LV2_STATE_SUCCESS;
}

SampleCache& LV2Plugin::get_sample_cache() {
    return retainer->getSampleCache();
}

} // namespace ksp1

static lvtk::Descriptor<ksp1::LV2Plugin> _KSP1 (KSP1_URI, { LV2_URID__map, LV2_WORKER__schedule });
