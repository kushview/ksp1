/*
    LayerData.cpp - This file is part of KSP1
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

#include "engine/LayerData.h"
#include "engine/LowPassFilter.h"
#include "engine/LV2Plugin.h"
#include "engine/SampleCache.h"
#include "engine/SamplerSynth.h"

namespace KSP1 {

    static int32 lastLayerSalt = 0;

    LayerData::LayerData (SampleCache& cache, int layerId)
        : cache (cache), id ((layerId != 0) ? layerId : KSP1::generateObjectID (++lastLayerSalt))
    {
        index = -1;
        note = -1;
        type = 0;
        pitch.set (0.0f);
        panning.set (0.5f);
        renderBuffer.set (nullptr);
        in.set (0),
        out.set (0);
        gain.set (1.0f);
        filter = new LowPassFilter ();
        sampleRate = 0.0f;
        numChannels = 0;
        lengthInSamples = 0;
    }

    LayerData::~LayerData()
    {
        filter = nullptr;
    }

    void LayerData::setAtomObject (const URIs& uris, const lvtk::AtomObject& object, bool realtime)
    {
        for (const auto& p : object)
            setProperty (uris, p.key, lvtk::Atom(&p.value), realtime);
    }

    void LayerData::setProperty (const URIs& uris, uint32_t prop, const lvtk::Atom& value, bool realtime)
    {
        if (prop == uris.slugs_volume)
        {
            gain.set (Decibels::decibelsToGain (value.as_double()));
        }
        else if (prop == uris.slugs_file)
        {
            if (! realtime)
            {
                const File file (String::fromUTF8 (value.as_string()));
                if (! this->loadAudioFile (file)) {
                    DBG ("LayerData::setProperty(): error loading file property: " << file.getFileName());
                }
            }
        }
        else if (prop == uris.slugs_velocityLower)
        {
            velocityRange.setStart (value.as_double());
        }
        else if (prop == uris.slugs_velocityUpper)
        {
            velocityRange.setEnd  (value.as_double());
        }
        else if (prop == uris.slugs_start)
        {
            in.set (sampleRate * value.as_double());
            out.set (in.get() + out.get());
        }
        else if (prop == uris.slugs_length)
        {
            out.set (in.get() + (sampleRate * value.as_double()));
        }
        else if (prop == uris.slugs_pitch)
        {
            pitch.set (value.as_double());
        }
        else if (prop == uris.slugs_panning) {
            panning.set (value.as_double());
        }
        else if (prop == uris.slugs_note)
            note = value.as_int();
        else if (prop == uris.slugs_index) {
            index = value.as_int();
        }
        else if (prop == uris.slugs_cutoff) {

        }
        else if (prop == uris.slugs_resonance) {

        }
        else if (prop == uris.slugs_offset) {

        }
        else if (prop == uris.slugs_name) {

        }
        else if (prop == uris.slugs_index) {
            index = value.as_int();
        }
        else if (prop == uris.slugs_parent) {
            parent = value.as_urid();
        }
        else {
            DBG ("unhandled property urid: " << (int)prop);
        }
    }

    void LayerData::setProperty (const URIs& uris, const PatchSet& set)
    {
        setProperty (uris, set.property.as_urid(), set.value, true);
    }

    void LayerData::startNote (int voice, const KeyInfo& key)
    {
        if (voice < 0) {
            return;
        }

        if (AudioSampleBuffer* buffer = renderBuffer.get())
        {
            if (out.get() > buffer->getNumSamples())
                out.set (buffer->getNumSamples());

            if (in.get() == out.get())
            {
                out.set (buffer->getNumSamples());

                if (in.get() >= out.get())
                    in.set (0);
            }
            else if (in.get() > out.get())
            {
                int64 oldOut = out.get();
                out.set (in.get());
                in.set (oldOut);
            }
        }
    }

    void LayerData::reset()
    {
        note = index = -1;
        if (renderBuffer.set (nullptr))
           scratch.reset();
    }

    void LayerData::restoreFromXml (const XmlElement& e)
    {
        index = e.getIntAttribute ("index", -1);
        note = e.getIntAttribute ("note", -1);
        gain.set (Decibels::decibelsToGain (e.getDoubleAttribute ("volume", gain.get())));
        panning.set (e.getDoubleAttribute ("panning", panning.get()));
        pitch.set (e.getDoubleAttribute ("pitch", pitch.get()));
        velocityRange.setStart (e.getDoubleAttribute ("velocityLower"));
        velocityRange.setEnd (e.getDoubleAttribute ("velocityUpper"));
        if (e.hasAttribute ("file"))
        {
            const String filePath (e.getStringAttribute ("file", currentFile.getFullPathName()));
            if (! loadAudioFile (File (filePath)))
            {
                //
            }
        }
    }

    ForgeRef LayerData::writeAtomObject (Forge& forge)
    {
        const URIs& uris (forge.uris);
        ForgeFrame frame;
        ForgeRef ref (forge.write_object (frame, (uint32) id, forge.uris.ksp1_LayerData));
        forge.write_key (uris.slugs_index); forge.write_int (index);
        forge.write_key (uris.slugs_note); forge.write_int (note);
        forge.write_key (uris.slugs_volume); forge.write_double (Decibels::gainToDecibels ((double) gain.get()));
        forge.write_key (uris.slugs_pitch); forge.write_double (pitch.get());
        forge.write_key (uris.slugs_panning); forge.write_double (panning.get());
        forge.write_key (uris.slugs_start); forge.write_double ((double) 0.0);
        forge.write_key (uris.slugs_length); forge.write_double ((double)(out.get() - in.get()) / sampleRate);
        forge.write_key (uris.slugs_offset); forge.write_double (in.get());
        forge.pop_frame (frame);
        return ref;
    }

    DynamicObject::Ptr LayerData::createDynamicObject() const
    {
        DynamicObject::Ptr object = new DynamicObject();
        object->setProperty (Slugs::id, id);
        object->setProperty (Slugs::index, index);
        object->setProperty (Slugs::note, note);
        object->setProperty (Slugs::volume, Decibels::gainToDecibels ((double) gain.get()));
        object->setProperty (Slugs::pitch, pitch.get());
        object->setProperty (Tags::panning, panning.get());
        object->setProperty (Tags::velocityLower, velocityRange.getStart());
        object->setProperty (Tags::velocityUpper, velocityRange.getEnd());
        object->setProperty (Slugs::start, (double) in.get() / sampleRate);
        object->setProperty (Slugs::length, (double)(out.get() - in.get()) / sampleRate);
        object->setProperty (Slugs::file, currentFile.getFullPathName());
        return object;
    }

    const float* LayerData::getSampleData (int32 chan, int32 frame) const
    {
        return renderBuffer.get() != nullptr ? renderBuffer.get()->getReadPointer (chan, frame)
                                             : nullptr;
    }

    bool LayerData::loadAudioFile (const File& audioFile)
    {
        BufferPtr old = scratch;
        scratch = cache.loadAudioFile (audioFile);

        //if (sampleRate == 0.0f)
        {
            if (ScopedPointer<AudioFormatReader> reader = cache.createReaderFor (audioFile))
            {
                if (sampleRate != reader->sampleRate)
                    sampleRate = reader->sampleRate;
                if (lengthInSamples != reader->lengthInSamples)
                    lengthInSamples = reader->lengthInSamples;
                if (numChannels != reader->numChannels)
                    numChannels = reader->numChannels;
            }
        }

        if (! audioFile.existsAsFile())
        {
            DBG (audioFile.getFullPathName() << "  DOES NOT EXIST");
        }
        currentFile = audioFile;

        if (scratch)
        {
            in.set (0);
            out.set (scratch->getNumSamples());
            while (! renderBuffer.set (scratch.get())) { }
        }
        else
        {
            DBG ("Buffer not acquired");
            while (! renderBuffer.set (nullptr));
        }

        old.reset();
        return renderBuffer.get() != nullptr;
    }
}
