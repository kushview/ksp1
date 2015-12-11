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

namespace Gui {
    class PluginEditor;
}
    
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
    LV2_URID map (const char* uri);
    
private:
    friend class PluginProcessor;
    Array<PluginProcessor*> instances;
    SymbolMap symbols;
    ScopedPointer<WorkThread> workThread;
    Array<File> factoryInstruments;
    
    void init();
};

class PluginProcessor  : public AudioProcessor,
                         private Timer
{
public:
    ~PluginProcessor();
    static PluginProcessor* create();

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
    void unregisterEditor (Gui::PluginEditor*);

protected:
    PluginProcessor();

private:
    bool useExternalData;
    ScopedPointer<PluginModule> module;
    ScopedPointer<RingBuffer> ring, uiRing;
    HeapBlock<uint8> block;

    ScopedPointer<PortBuffer> atomIn, atomOut;
    Array<Gui::PluginEditor*> editors;
    int currentProgram;
    
    friend class Timer;
    void timerCallback() override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor);
};

}

#endif  // KSP1_PLUGIN_PROCESSOR_H
