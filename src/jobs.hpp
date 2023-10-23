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

#ifndef KSP1_JOBS_H
#define KSP1_JOBS_H

#include "juce.hpp"

#include <lvtk/ext/atom.hpp>
#include <lvtk/ext/worker.hpp>
#include <lvtk/memory.hpp>

#include "ksp1.hpp"
#include "urids.hpp"

namespace ksp1 {

class Disposable;
class JobManager;
class LV2Plugin;
class SampleCache;
class SamplerSynth;

class Job {
public:
    virtual ~Job() {}

protected:
    Job() noexcept {}
    LV2_Atom_Forge* getForge() const { return forge; }
    SampleCache* getSampleCache() const { return cache; }

    void respond (const lvtk::Atom& atom);
    void respond (uint32_t, const void*);
    virtual void work (const URIs& uris, const lvtk::Atom& atom) = 0;

    JobManager* jobs { nullptr };

private:
    lvtk::WorkerRespond* workerRespond { nullptr };
    LV2_Atom_Forge* forge { nullptr };
    SampleCache* cache { nullptr };

    void executeWork (const URIs& uris, const lvtk::Atom& atom) {
        work (uris, atom);
    }

    friend class JobManager;
};

struct SamplerSynthMessage {
    LV2_Atom atom;
    SamplerSynth* samplerSynth;
};

class JobType {
public:
    JobType() {}
    virtual ~JobType() {}
    virtual bool canWork (const URIs& uris, const lvtk::Atom&)              = 0;
    virtual void createJobsFor (const URIs& uris,
                                const lvtk::Atom&, juce::Array<Job*>& jobs) = 0;
};

class JobManager {
public:
    JobManager (LV2Plugin& p);
    ~JobManager();

    lvtk::WorkerStatus work (lvtk::WorkerRespond&, const URIs&, const lvtk::Atom&);
    void registerType (JobType*);
    void finalize (Job* job);

    void dispose (Disposable*);

    LV2_Atom_Forge& getWorkForge() {
        if (workForgeBuf.getSize() == 0)
            workForgeBuf.setSize (4096);
        lv2_atom_forge_set_buffer (&workForge,
                                   (uint8_t*) workForgeBuf.getData(),
                                   workForgeBuf.getSize());
        return workForge;
    }

private:
    LV2Plugin& plugin;
    LV2_Atom_Forge forge, workForge;
    juce::MemoryBlock workForgeBuf;
    juce::OwnedArray<JobType> types;
    juce::OwnedArray<Job> working;
    void scheduleWork (uint32_t, const void*);
    void scheduleWork (const lvtk::Atom& atom);
};

/** Helper class for dealing with referenced objects
    This class inherits lvtk::Atom, so you can pass it to lvtk and
    LV2 standard functions that take LV2_Atom* as parameters
 */
struct ObjectRef : public lvtk::Atom {
    /** Writes writes an object pointer by using a Forge.  The passed
        in Forge must have already called lvtk::AtomForge::set_buffer
        before using this ctor
     */
    ObjectRef (LV2_Atom_Forge& forge, const URIs& urids, uint32_t class_type, void* ptr)
        : lvtk::Atom (write_object (forge, urids, class_type, ptr)) {
        p_class_ptr = lvtk::read_unaligned<intptr_t> (ptr);
    }

    /** Read an ObjectRef from an AtomObject */
    ObjectRef (const URIs& uris, const LV2_Atom_Object* object)
        : lvtk::Atom (object) {
        jassert (object->body.otype == uris.jobs_ObjectRef);
        const LV2_Atom* class_type = nullptr;
        const LV2_Atom* object_ptr = nullptr;
        lv2_atom_object_get (object,
                             uris.slugs_type,
                             &class_type,
                             uris.slugs_object,
                             &object_ptr,
                             0);

        p_class_ptr  = (object_ptr) ? lvtk::read_unaligned<intptr_t> (LV2_ATOM_BODY (object_ptr)) : 0;
        m_class_type = (class_type) ? lvtk::read_unaligned<uint32_t> (LV2_ATOM_BODY (class_type)) : 0;
    }

    /** Returns the stored class pointer casted to an OType */
    template <class OType>
    OType* get() const { return static_cast<OType*> ((void*) p_class_ptr); }

    /** Returns the stored class type URID */
    uint32_t get_class_type() const { return m_class_type; }

    /** Returns true if the stored class type equals the test value.
        @param t The class type id to test
     */
    bool has_class_type (const uint32_t t) const { return m_class_type == t; }

    /** Returns the required space in bytes that an ObjectRef needs
        to write an atom object with
     */
    static uint32_t required_space() { return 64; }

private:
    intptr_t p_class_ptr;
    uint32_t m_class_type;

    static LV2_Atom_Forge_Ref write_object (LV2_Atom_Forge& f,
                                            const URIs& uris,
                                            uint32_t class_type,
                                            void* ptr) {
        LV2_Atom_Forge_Frame frame;

        auto ref = lv2_atom_forge_object (&f, &frame, 0, uris.jobs_ObjectRef);
        lv2_atom_forge_key (&f, uris.slugs_type);
        lv2_atom_forge_urid (&f, class_type);
        lv2_atom_forge_key (&f, uris.slugs_object);
        lv2_atom_forge_long (&f, (intptr_t) ptr); //!!! not exactly right, but it works.
        lv2_atom_forge_pop (&f, &frame);

        return ref;
    }
};

} // namespace ksp1

#endif // KSP1_JOBS_H
