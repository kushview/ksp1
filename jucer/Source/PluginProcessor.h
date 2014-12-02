/*
    PluginProcessor.h - This file is part of KSP1

    Copyright (C) 2013 Kushview, LLC  All rights reserved.
      * Michael Fisher <mfisher@kushview.net>
*/


#ifndef KSP1_H
#define KSP1_H

#include "PluginConfig.h"
#include "Instrument.h"
#include "Sampler.h"

namespace KSP1 {

class AudioEngine;
class Globals;

class PluginProcessor  : public AudioProcessor
{
public:

    PluginProcessor (KSP1::Globals& world);
    PluginProcessor();
    ~PluginProcessor();

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

    void fillInPluginDescription (PluginDescription &description) const;

    KSP1::InstrumentPtr getInstrument() const { return this->instrument; }
    KSP1::SamplerProcessor* getSampler() const { return sampler.get(); }
    
private:
    bool useExternalData;
    KSP1::InstrumentPtr instrument;
    ScopedPointer<KSP1::SamplerProcessor> sampler;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};

}

#endif  // KSP1_H
