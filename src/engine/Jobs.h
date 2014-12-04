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
#include "Forge.h"
#include "KSP1.h"
#include "URIs.h"
#include "engine/SamplerSynth.h"
#include "../../libs/lvtk/lvtk/ext/atom.hpp"
#include "../../libs/lvtk/lvtk/ext/worker.hpp"

namespace lvtk {
    struct Atom;
    struct AtomObject;
    struct WorkerRespond;
}

namespace KSP1 {

class JobManager;
class LV2Plugin;

class Job {
public:
    virtual ~Job() { }

protected:
    Job () : workerRespond (nullptr), forge(nullptr), cache(nullptr) { }
    Forge* getForge() const { return forge; }
    SampleCache* getSampleCache() const { return cache; }

    void respond (const lvtk::Atom& atom);
    void respond (uint32_t, const void*);
    virtual void work (const URIs& uris, const lvtk::Atom& atom) = 0;

    JobManager* jobs;

private:
    lvtk::WorkerRespond* workerRespond;
    Forge* forge;
    SampleCache* cache;

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
    JobType() { }
    virtual ~JobType() { }
    virtual bool canWork (const URIs&  uris, const lvtk::Atom&) = 0;
    virtual void createJobsFor (const URIs& uris, const lvtk::Atom&, Array<Job*>& jobs) = 0;
};

class JobManager {
public:

    JobManager (LV2Plugin& p);
    ~JobManager();

    lvtk::WorkerStatus work (lvtk::WorkerRespond&, const URIs&, const lvtk::Atom&);
    void registerType (JobType*);
    void finalize (Job* job);

    void dispose (Disposable*);

    Forge& getWorkForge ()
    {
        if (workForgeBuf.getSize() == 0)
            workForgeBuf.setSize (2048);

        workForge.set_buffer ((uint8*) workForgeBuf.getData(), workForgeBuf.getSize());
        return workForge;
    }

private:
    LV2Plugin& plugin;
    Forge forge, workForge;
    MemoryBlock workForgeBuf;
    OwnedArray<JobType> types;
    OwnedArray<Job> working;
    void scheduleWork (uint32_t, const void*);
    void scheduleWork (const lvtk::Atom& atom);

};

/** Helper class for dealing with referenced objects
    This class inherits lvtk::Atom, so you can pass it to lvtk and
    LV2 standard functions that take LV2_Atom* as parameters
 */
struct ObjectRef : public lvtk::Atom
{

    /** Writes writes an object pointer by using a Forge.  The passed
        in Forge must have already called lvtk::AtomForge::set_buffer
        before using this ctor
     */
    ObjectRef (Forge& forge, uint32_t class_type, void* ptr)
        : lvtk::Atom (write_object (forge, class_type, ptr))
    {
        p_class_ptr = (intptr_t) ptr;
    }

    /** Read an ObjectRef from an AtomObject */
    ObjectRef (const URIs& uris, const lvtk::AtomObject& object)
        : lvtk::Atom (object)
    {
        jassert (object.otype() == uris.jobs_ObjectRef);
        const lvtk::Atom class_type, object_ptr;
        lv2_atom_object_get (object,
            uris.slugs_type, &class_type,
            uris.slugs_object, &object_ptr,
        0);

        p_class_ptr  = (object_ptr) ? *static_cast<intptr_t*> (object_ptr.body()) : 0;
        m_class_type = (class_type) ? class_type.as_urid() : 0;
    }

    /** Returns the stored class pointer casted to an OType */
    template<class OType> OType* get() const { return static_cast<OType*> ((void*) p_class_ptr); }

    /** Returns the stored class type URID */
    uint32_t get_class_type() const { return m_class_type; }

    /** Returns true if the stored class type equals the test value.
        @param t The class type id to test
     */
    bool has_class_type (const uint32_t t) const { return m_class_type == t; }

    /** Returns the required space in bytes that an ObjectRef needs
        to write an atom object with
     */
    static uint32_t required_space () { return 64; }

private:
    intptr_t p_class_ptr;
    uint32_t m_class_type;
    static ForgeRef write_object (Forge& f, uint32_t class_type, void *ptr)
    {
        ForgeFrame frame;
        ForgeRef ref (f.write_object (frame, 0, f.uris.jobs_ObjectRef));

        f.write_key (f.uris.slugs_type);
        f.write_urid (class_type);
        f.write_key (f.uris.slugs_object);
        f.write_long ((intptr_t) ptr); //!!! not exactly right, but it works.
        f.pop (frame);

        return ref;
    }
};

}

#endif // KSP1_JOBS_H
