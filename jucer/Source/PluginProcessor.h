/*
    PluginProcessor.h - This file is part of KSP1

    Copyright (C) 2013 Kushview, LLC  All rights reserved.
      * Michael Fisher <mfisher@kushview.net>
*/

#ifndef KSP1_PLUGIN_PROCESSOR_H
#define KSP1_PLUGIN_PROCESSOR_H

#include "Instrument.h"

namespace KSP1 {

class PluginModule;
class PluginProcessor;

class PluginWorld
{
public:
    PluginWorld();
    ~PluginWorld();

    LV2Feature* createMapFeature();
    LV2Feature* createUnmapFeature();
    void registerPlugin (PluginProcessor* plug);
    bool unregisterPlugin (PluginProcessor* plug);
    AudioProcessor* load (const String& uri);
    WorkThread& getWorkThread();

private:
    Array<PluginProcessor*> instances;
    SymbolMap symbols;
    ScopedPointer<WorkThread> workThread;
    void init();
};

class PluginProcessor  : public AudioProcessor
{
public:
    PluginProcessor();
    ~PluginProcessor();

    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();

    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);
    void writeToPort (uint32 portIndex, uint32 bufferSize, uint32 portProtocol, const void* buffer);
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
    
private:
    bool useExternalData;
    ScopedPointer<PluginModule> module;
    ScopedPointer<RingBuffer> ring;
    HeapBlock<uint8> block;
    HeapBlock<uint8> seqBlock;
    ScopedPointer<PortBuffer> atomIn, atomOut;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor);
};

}

#endif  // KSP1_PLUGIN_PROCESSOR_H
