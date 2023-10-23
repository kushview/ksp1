/*
    Jobs.cpp - This file is part of KSP1
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

#include <lvtk/ext/worker.hpp>

#include "disposable.hpp"
#include "jobs.hpp"

namespace ksp1 {

class ObjectDisposalJobType : public JobType {
public:
    ObjectDisposalJobType() {}
    ~ObjectDisposalJobType() {}

    bool canWork (const URIs& uris, const lvtk::Atom& atom) override {
        if (atom.as_object().otype() != uris.jobs_ObjectRef)
            return false;

        ObjectRef ref (uris, atom.as_object());
        return ref.has_class_type (uris.jobs_Disposable);
    }

    void createJobsFor (const URIs& uris, const lvtk::Atom& atom, juce::Array<Job*>& jobs) override {
        jobs.add (new ObjectDeleter());
    }

    class ObjectDeleter : public Job {
    public:
        ObjectDeleter() {}
        ~ObjectDeleter() {}

        void work (const URIs& uris, const lvtk::Atom& atom) override {
            ObjectRef ref (uris, atom.as_object());
            if (Disposable* ptr = ref.get<Disposable>())
                delete ptr;
        }
    };
};

class MediaLoaderJobType : public JobType {
public:
    MediaLoaderJobType() {}
    ~MediaLoaderJobType() {}

    bool canWork (const URIs& uris, const lvtk::Atom& atom) {
        if (atom.type() != uris.atom_Object)
            return false;
        return false;
#if 0
        if (atom.has_object_type (uris.ksp1_Layer) || atom.has_object_type (uris.ksp1_Key))
            return true;

        if (atom.as_object().otype() != uris.patch_Set)
            return false;

        const PatchSet set (uris, atom.as_object());
        return (set.subject.type() == uris.atom_URID && set.subject.as_urid() == uris.ksp1_Instrument && set.property.as_urid() == uris.slugs_file && set.value.type() == uris.atom_Path);
#endif
    }

    void createJobsFor (const URIs& uris, const lvtk::Atom& atom, juce::Array<Job*>& jobs) {
#if 0
        if (atom.has_object_type (uris.patch_Set)) {
            const PatchSet set (uris, atom.as_object());
            jobs.add (new LoadSynthFile (set.value));
        }
        if (atom.has_object_type (uris.ksp1_Layer)) {
            jobs.add (new LoadLayerData());
        }
        if (atom.has_object_type (uris.ksp1_Key)) {
            jobs.add (new LoadSound());
        }
#endif
    }

    class LoadSound : public Job {
    public:
        LoadSound() {}
        ~LoadSound() {}
        void work (const URIs& uris, const lvtk::Atom& atom) override {
#if 0
            const lvtk::AtomObject object (atom.as_object());
            const lvtk::Atom note;
            lv2_atom_object_get (object, uris.slugs_note, &note, 0);
            if (note) {
                SamplerSound* s = new SamplerSound (note.as_int(), static_cast<int> (object.id()));

                const ObjectRef ref (jobs->getWorkForge(), uris.ksp1_Key, s);
                if (ref.get<SamplerSound>() != nullptr) {
                    respond (ref.total_size(), ref.cobj());
                }
            }
#endif
        }
    };

    class LoadLayerData : public Job {
    public:
        LoadLayerData() {}
        ~LoadLayerData() {}
        void work (const URIs& uris, const lvtk::Atom& atom) override {
#if 0
            SampleCache& cache (*getSampleCache());
            if (LayerData* data = cache.getLayerData (uris, atom.as_object(), false)) {
                const ObjectRef ref (jobs->getWorkForge(), uris.ksp1_Layer, data);
                if (ref.get<LayerData>() != nullptr) {
                    respond (ref.total_size(), ref.cobj());
                }
            }
#endif
        }
    };

    class LoadSynthFile : public Job {
    public:
        LoadSynthFile (const lvtk::Atom& patchValue) : path (patchValue.as_string()) {}
        ~LoadSynthFile() {}

        void work (const URIs& uris, const lvtk::Atom& atom) override {
#if 0
            const File file (String::fromUTF8 (path.c_str()));
            if (ScopedXml xml = XmlDocument::parse (file)) {
                if (ScopedPointer<SamplerSynth> ss = SamplerSynth::create()) {
                    if (ss->loadValueTreeXml (*xml)) {
#    define compressd_json 1
#    if compressd_json

                        var json;
                        const bool haveJson = ss->getNestedVariant (json);
#    endif
                        uint8_t buf[2048];
                        getForge()->set_buffer (buf, 2048);
                        ObjectRef synth (*getForge(), uris.ksp1_SamplerSynth, ss.release());
                        respond (synth);

                        if (haveJson) {
                            MemoryBlock block (0, false);
                            MemoryOutputStream stream (block, false);
                            GZIPCompressorOutputStream compressed (&stream, 9);
                            JSON::writeToStream (compressed, json, true);
                            compressed.flush();

                            if (block.getSize() > 0) {
                                Forge& f    = *getForge();
                                int bufSize = nextPowerOfTwo ((int) block.getSize());
                                bufSize     = nextPowerOfTwo (bufSize);
                                MemoryBlock buffer ((size_t) bufSize, false);
                                f.set_buffer ((uint8*) buffer.getData(), buffer.getSize());
                                lvtk::Atom gzipped (f.write_atom (block.getSize(), 100100));
                                f.write_raw (block.getData(), block.getSize());
                                DBG ("SIZE: " << (int) block.getSize());
                                respond (gzipped);
                            }
                        }
                    }
                }
            }
#endif
        }

        const std::string path;
    };
};

void Job::respond (uint32_t s, const void* b) {
    if (workerRespond)
        (*workerRespond) (s, b);
}

void Job::respond (const lvtk::Atom& atom) {
    respond (atom.total_size(), atom.c_obj());
}

JobManager::JobManager (LV2Plugin& p)
    : plugin (p) {
    registerType (new ObjectDisposalJobType());
    registerType (new MediaLoaderJobType());
}

JobManager::~JobManager() {}

void JobManager::finalize (Job* job) {
    jassert (working.contains (job));
    working.removeObject (job);
}

lvtk::WorkerStatus JobManager::work (lvtk::WorkerRespond& respond, const URIs& uris, const lvtk::Atom& atom) {
    juce::Array<Job*> jobs;

    for (JobType* type : types)
        if (type->canWork (uris, atom))
            type->createJobsFor (uris, atom, jobs);

    for (Job* job : jobs) {
        working.add (job);
        job->jobs = this;
        auto& jobForge (getWorkForge()); // prepares the buffer
        job->forge         = &jobForge;
        job->workerRespond = &respond;
        job->cache         = nullptr; //&plugin.get_sample_cache();
        job->executeWork (uris, atom);
        job->workerRespond = nullptr;
        job->forge         = nullptr;
        job->cache         = nullptr;
    }

    //!!! just clear everything for now.
    working.clearQuick (true);
    return LV2_WORKER_SUCCESS;
}

void JobManager::dispose (Disposable* obj) {
#if 0
    uint8_t buf[128];
    lv2_atom_forge_set_buffer (&forge, buf, 128);

    const auto& uris = plugin.get_uris();
    ObjectRef ref (forge, uris, uris.jobs_Disposable, obj);
    scheduleWork (ref);
#endif
}

void JobManager::registerType (JobType* type) {
    types.add (type);
}

void JobManager::scheduleWork (uint32_t s, const void* d) {
    // plugin.schedule_work (s, d);
}

void JobManager::scheduleWork (const lvtk::Atom& atom) {
    // plugin.schedule_work (atom.total_size(), atom.c_obj());
}

} // namespace ksp1
