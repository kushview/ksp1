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

#include "DirectInterface.h"
#include "editor/SamplerEditor.h"
#include "engine/Articulator.h"
#include "Filesystem.h"
#include "engine/SampleCache.h"
#include "engine/SamplerKeys.h"
#include "engine/SamplerSynth.h"
#include "Instrument.h"
#include "Sampler.h"

#include "KSP1.h"
#include "Globals.h"


#define KSP1_MIDIFILTERS 0
#define KSP1_TRACE_MIDI 0

namespace KSP1 {

    SamplerProcessor::SamplerProcessor (Globals& g)
        : render (1, 1)
    {
        world.setNonOwned (&g);
        sampler.setOwned (new DirectInterface (world->sampleCache()));
        init();
    }

    SamplerProcessor::SamplerProcessor()
        : render (1, 1)
    {
        world.setOwned (new Globals());
        sampler.setOwned (new DirectInterface (world->sampleCache()));
        init();
    }

    SamplerProcessor::~SamplerProcessor()
    {
        state.removeListener (this);
        articulator = nullptr;
        midiFilters.clearQuick (true);
        sampler.clear();
        world.clear();
    }

    SamplerInterface* SamplerProcessor::currentSynth()
    {
        return dynamic_cast<SamplerInterface*> (sampler.get());
    }

    void SamplerProcessor::init()
    {
         setGain (1.0);
         midiFilters.add (articulator = new Articulator());
         state.addListener (this);
    }

    void
    SamplerProcessor::setGain (float gain)
    {
        sampler->setGain (gain);
    }

    void
    SamplerProcessor::setInstrument (InstrumentPtr instrument)
    {
        sampler->setInstrument (instrument);
        if (InstrumentPtr i = sampler->getInstrument())
            state = i->node();
        else
            state = ValueTree::invalid;
    }

    void
    SamplerProcessor::setMidiChannel (int32 channel)
    {
        if (sampler->getMidiChannel() != channel)
            sampler->setMidiChannel (channel);
    }

    const
    String SamplerProcessor::getName() const
    {
        return "Sampler";
    }

    int SamplerProcessor::getNumParameters()
    {
        return 0;
    }

    float SamplerProcessor::getParameter (int index)
    {
        return 0.0f;
    }

    void SamplerProcessor::setParameter (int index, float newValue)
    {
    }

    const
    String SamplerProcessor::getParameterName (int index)
    {
        return String::empty;
    }

    const
    String SamplerProcessor::getParameterText (int index)
    {
        return String::empty;
    }

    const String SamplerProcessor::getInputChannelName (int chan) const
    {
        return String ("Audio ") + String (chan + 1);
    }

    const String
    SamplerProcessor::getOutputChannelName (int chan) const
    {
        return String ("Audio ") + String (chan + 1);
    }

    bool
    SamplerProcessor::isInputChannelStereoPair (int index) const
    {
        return true;
    }

    bool
    SamplerProcessor::isOutputChannelStereoPair (int index) const
    {
        return true;
    }

    bool SamplerProcessor::acceptsMidi() const { return true; }
    bool SamplerProcessor::producesMidi() const { return true; }

    bool
    SamplerProcessor::silenceInProducesSilenceOut() const
    {
        return false;
    }

    double
    SamplerProcessor::getTailLengthSeconds() const
    {
        return 0.0;
    }

    int
    SamplerProcessor::getNumPrograms()
    {
        return 0;
    }

    int
    SamplerProcessor::getCurrentProgram()
    {
        return 0;
    }

    void
    SamplerProcessor::setCurrentProgram (int index)
    {
    }

    const
    String SamplerProcessor::getProgramName (int index)
    {
        return String::empty;
    }

    void
    SamplerProcessor::changeProgramName (int index, const String& newName)
    {

    }

    void
    SamplerProcessor::prepareToPlay (double sampleRate, int blockSize)
    {
        for (MidiFilter* mf : midiFilters)
            mf->prepareToPlay (sampleRate, blockSize);

        sampler->setCurrentPlaybackSampleRate (sampleRate);
        setPlayConfigDetails (0, 2, sampleRate, blockSize);
        render.setSize (10, blockSize, false, true, false);
        lastGain = sampler->gain();
    }

    void
    SamplerProcessor::releaseResources()
    {
        for (MidiFilter* mf : midiFilters)
            mf->releaseResources();

        render.setSize (1, 1);
    }

    void
    SamplerProcessor::processBlock (AudioSampleBuffer& audio, MidiBuffer& midi)
    {        
        const int32 nframes = audio.getNumSamples();

        {
            MidiBuffer m;
            midiKeyboard.processNextMidiBuffer (m, 0, nframes, true);
            midi.addEvents (m, 0, nframes, 0);
        }

       #if KSP1_MIDIFILTERS
        for (MidiFilter* mf : midiFilters)
            mf->processMidi (midi, nframes);
       #endif

        render.setSize (10, nframes, false, false);
        for (int32 c = 0; c < 2; ++c)
            render.clear (c, 0, nframes);
        
       #if KSP1_TRACE_MIDI
        MidiBuffer::Iterator iter (midi);
        MidiMessage m; int p;
        while (iter.getNextEvent (m, p)) {
            if (m.isNoteOn()) {
                std::clog << "note on\n";
            } else if (m.isNoteOff()) {
                std::clog << "note off\n";
            }
        }
       #endif
        
        sampler->renderNextBlock (render, midi, 0, nframes);

        for (int c = 0; c < 2; ++c)
        {
            audio.copyFromWithRamp (c, 0, render.getReadPointer (c, 0),
                nframes, lastGain, sampler->gain());
        }

        lastGain = sampler->gain();
        peakL.set (audio.getMagnitude (0, 0, audio.getNumSamples()));
        peakR.set (audio.getMagnitude (1, 0, audio.getNumSamples()));

        midi.clear();
    }

    bool
    SamplerProcessor::hasEditor() const
    {
        return true;
    }

    AudioProcessorEditor*
    SamplerProcessor::createEditor()
    {
        if (Gui::SamplerEditor* ed = new Gui::SamplerEditor (this))
            return ed;
        return nullptr;
    }

    void SamplerProcessor::getStateInformation (MemoryBlock& destData) { }
    void SamplerProcessor::setStateInformation (const void* data, int sizeInBytes) { }

    void
    SamplerProcessor::fillInPluginDescription (PluginDescription& desc) const
    {
        desc.name               = getName();
        desc.category           = "Instruments";
        desc.fileOrIdentifier   = KSP1_SAMPLER_URI;
        desc.isInstrument       = true;
        desc.manufacturerName   = "Kushview, LLC";
        desc.numInputChannels   = 0;
        desc.numOutputChannels  = 2;
        desc.uid                = desc.fileOrIdentifier.hashCode();
        desc.version            = "0.0.1";
        desc.pluginFormatName   = "Internal";
    }

    void SamplerProcessor::valueTreePropertyChanged (ValueTree& tree, const Identifier& prop)
    {
        if (tree.hasType (Tags::articulation))
        {
            KeyItem pad (tree.getParent());
            StrokeType stroke (tree.getProperty (prop, var::null));
            articulator->setStrokeType (pad.note(), stroke);
        }
    }

    void SamplerProcessor::valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) { }
    void SamplerProcessor::valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved) { }
    void SamplerProcessor::valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved) { }
    void SamplerProcessor::valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged) { }
    void SamplerProcessor::valueTreeRedirected (ValueTree& treeWhichHasBeenChanged) { }

}
