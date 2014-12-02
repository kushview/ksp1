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

#ifndef KSP1_SAMPLER_PROCESSOR_H
#define KSP1_SAMPLER_PROCESSOR_H

#include "Instrument.h"
#include "engine/SamplerSynth.h"
#include "KSP1.h"
#include "Globals.h"

#define KSP1_SAMPLER_URI "http://bketech.com/processors/sampler"
#define KSP1_SAMPLER_PREFIX KSP1_SAMPLER_URI "#"

#define KSP1_SAMPLER_Key      KSP1_SAMPLER_PREFIX "Key"
#define KSP1_SAMPLER_Layer    KSP1_SAMPLER_PREFIX "Layer"
#define KSP1_SAMPLER_Layers   KSP1_SAMPLER_PREFIX "Layers"
#define KSP1_SAMPLER_Asset    KSP1_SAMPLER_PREFIX "Asset"
#define KSP1_SAMPLER_loadKit  KSP1_SAMPLER_PREFIX "loadKit"

namespace KSP1 {

    class Articulator;
    class MidiFilter;
    class DirectInterface;

    /** A standalone sampler processor */
    class SamplerProcessor :  public AudioProcessor,
                              private ValueTree::Listener
    {
    public:

        enum PortID {
            MainOut1 = 0,
            MainOut2,
            ControlOut,
            ControlIn,
            NumPorts
        };


        /** Create a sampler that uses shared Global data */
        SamplerProcessor (Globals& g);
        
        /** Create a sampler that uses internal non-shared data */
        SamplerProcessor();

        ~SamplerProcessor();

        inline uint32 getNumPorts() const { return (uint32) NumPorts;}

        inline lvtk::PortType
        getPortType (uint32 port)
        {
            if (port <= MainOut2)
                return lvtk::PortType::Audio;
            if (port >= ControlOut && port <= ControlIn)
                return lvtk::PortType::Atom;
            return lvtk::PortType::Unknown;
        }

        inline bool
        isPortInput (uint32 port)
        {
            jassert (port < NumPorts);
            return (port == ControlIn);
        }

        void prepareToPlay (double sampleRate, int samplesPerBlock);
        void releaseResources();

        void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

        AudioProcessorEditor* createEditor();
        bool hasEditor() const;

        const String getName() const;

        int getNumParameters();

        float getParameter (int index);
        void setParameter (int index, float newValue);

        const String getParameterName (int index);
        const String getParameterText (int index);

        const String getInputChannelName (int channelIndex) const;
        const String getOutputChannelName (int channelIndex) const;
        bool isInputChannelStereoPair (int index) const;
        bool isOutputChannelStereoPair (int index) const;

        bool acceptsMidi() const;
        bool producesMidi() const;
        bool silenceInProducesSilenceOut() const;
        double getTailLengthSeconds() const;

        int getNumPrograms();
        int getCurrentProgram();
        void setCurrentProgram (int index);
        const String getProgramName (int index);
        void changeProgramName (int index, const String& newName);

        void getStateInformation (MemoryBlock& destData);
        void setStateInformation (const void* data, int sizeInBytes);

        void setGain (float gain);
        void setInstrument (InstrumentPtr instrument);
        void setMidiChannel (int32 channel);

        // this is for the 'Internal' plugin format
        virtual void fillInPluginDescription (PluginDescription&) const;

        Globals& globals() { return *world; }

        inline void
        getMainPeaks (float& left, float& right)
        {
            left  = peakL.get();
            right = peakR.get();
        }

        MidiKeyboardState& keyboard() { return midiKeyboard; }
        SamplerInterface* currentSynth();

    private:
        MidiKeyboardState midiKeyboard, realtimeKeyboard;
        OptionalScopedPointer<DirectInterface> sampler;
        OptionalScopedPointer<KSP1::Globals> world;
        Element::AtomicValue<float> peakL, peakR;
        AudioSampleBuffer render;
        float lastGain;

        OwnedArray<MidiFilter> midiFilters;
        Articulator* articulator;

        friend class ValueTree;
        ValueTree state;
        void valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property);
        void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded);
        void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved);
        void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved);
        void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged);
        void valueTreeRedirected (ValueTree& treeWhichHasBeenChanged);

        void init();
    };

} /* namespace KSP1 */

#endif /* KSP1_SAMPLER_PROCESSOR_H */
