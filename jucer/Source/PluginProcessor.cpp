/*
    PluginProcessor.cpp - This file is part of KSP1

    Copyright (C) 2013 Kushview, LLC  All rights reserved.
      * Michael Fisher <mfisher@kushview.net>
*/

#include "../../lvtk/lvtk/plugin.hpp"
#include "../../lvtk/lvtk/ui.hpp"
#include "KSP1.h"
#include "engine/SamplerSynth.h"
#include "Locations.h"
#include "PluginEditor.h"
#include "PluginProcessor.h"
#include "Ports.h"

namespace KSP1
{    
    PluginWorld::PluginWorld() { }
    PluginWorld::~PluginWorld()
    {
        symbols.clear();
        workThread = nullptr;
    }

    LV2Feature* PluginWorld::createMapFeature()   { return symbols.createMapFeature(); }
    LV2Feature* PluginWorld::createUnmapFeature() { return symbols.createUnmapFeature(); }

    void PluginWorld::registerPlugin (PluginProcessor* plug)
    {
        if (instances.size() == 0)
            init();
            
        instances.addIfNotAlreadyThere (plug);
    }
        
    bool PluginWorld::unregisterPlugin(PluginProcessor* plug)
        {
            instances.removeFirstMatchingValue (plug);
            return instances.size() == 0;
        }
        
    AudioProcessor* PluginWorld::load(const String& uri)
        {
            return nullptr;
        }
    
    WorkThread& PluginWorld::getWorkThread()
    {
        jassert(workThread);
        return *workThread;
    }

    void PluginWorld::init()
    {
        workThread = new WorkThread ("KSP1_Worker", 4096, 5);
        workThread->startThread();
    }
   
    static ScopedPointer<PluginWorld> globals;
    
    class PluginModule
    {
    public:
        PluginModule()
            : descriptor (&lvtk::get_lv2_descriptors()[0]),
              uiDescriptor (&lvtk::get_lv2g2g_descriptors()[0]),
              handle (nullptr)
        {
            currentSampleRate = 0.0;
        }
        
        void instantiate (double rate)
        {
            bool rateChanged = false;
            if (currentSampleRate != rate)
            {
                currentSampleRate = rate;
                rateChanged = true;
            }
            
            if (handle != nullptr || rateChanged)
            {
                deactivate();
                cleanup();
                features = nullptr;
                jassert (nullptr == handle && nullptr == workerInterface);
            }
            
            if (nullptr == handle)
            {
                features = new LV2FeatureArray();
                features->add (globals->createMapFeature(), false);
                features->add (globals->createUnmapFeature());
                LV2Worker* worker = new LV2Worker (globals->getWorkThread(), 2048);
                features->add (worker);
                handle = descriptor->instantiate (descriptor, currentSampleRate, "", *features);
                workerInterface = const_cast<LV2_Worker_Interface*> (
                    (const LV2_Worker_Interface*) extensionData (LV2_WORKER__interface)
                );
                
                if (handle && workerInterface)
                    worker->setInterface (handle, workerInterface);
            }
        }
        
        void activate()
        {
            if (handle)
            {
                descriptor->activate (handle);
            }
        }
        
        void deactivate()
        {
            if (handle)
            {
                descriptor->deactivate (handle);
            }
        }
        
        void cleanup()
        {
            if (handle)
            {
                descriptor->cleanup (handle);
                handle = nullptr;
            }
            workerInterface = nullptr;
        }
        
        const void* extensionData (const String& uri)
        {
            return descriptor->extension_data (uri.toRawUTF8());
        }
        
        void process (AudioSampleBuffer& audio, MidiBuffer& midi)
        {
            if (! handle)
            {
                audio.clear(); midi.clear();
                return;
            }
            
            descriptor->connect_port (handle, Port::MainLeft, audio.getWritePointer (0, 0));
            descriptor->connect_port (handle, Port::MainRight, audio.getWritePointer (1, 0));
            
            midi.clear();
        }
       

    private:
        const LV2_Descriptor* descriptor;
        const LV2UI_Descriptor* uiDescriptor;
        LV2_Handle handle;
        LV2UI_Handle uiHandle;
        LV2_Worker_Interface* workerInterface;
        ScopedPointer<LV2FeatureArray> features;
        double currentSampleRate;
    };
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    using namespace KSP1;

    if (! globals) {
        globals = new PluginWorld();
    }
    
    PluginProcessor* plugin = new PluginProcessor();
    return plugin;
}

namespace KSP1 {

PluginProcessor::PluginProcessor()
{
    jassert (lvtk::get_lv2_descriptors().size() == 1);
    jassert (lvtk::get_lv2g2g_descriptors().size() == 1);
    module = new PluginModule();
    globals->registerPlugin (this);
}

PluginProcessor::~PluginProcessor()
{
    module->deactivate();
    module->cleanup();
    module = nullptr;

    const bool shutdownGlobals = globals->unregisterPlugin (this);
    if (shutdownGlobals)
    {
        globals = nullptr;
    }
}

void PluginProcessor::fillInPluginDescription (PluginDescription &desc) const
{
    desc.name               = getName();
    desc.category           = "Instruments";
    desc.fileOrIdentifier   = "http://kushview.net/ns/plugins/ksp1";
    desc.isInstrument       = true;
    desc.manufacturerName   = "Kushview, LLC";
    desc.numInputChannels   = 0;
    desc.numOutputChannels  = 2;
    desc.uid                = desc.fileOrIdentifier.hashCode();
    desc.version            = "0.0.1";
    desc.pluginFormatName   = "Internal";
}

const String PluginProcessor::getName() const {
    return JucePlugin_Name;
}

int PluginProcessor::getNumParameters() {
    return 0; // sampler->getNumParameters();
}

float PluginProcessor::getParameter (int index) {
    return 0.0f; // sampler->getParameter(index);
}

void PluginProcessor::setParameter (int index, float newValue) {
   // sampler->setParameter (index, newValue);
}

const String PluginProcessor::getParameterName (int index) {
    return "param name"; // sampler->getParameterName(index);
}

const String PluginProcessor::getParameterText (int index) {
    return "param text"; // sampler->getParameterText(index);
}

const String PluginProcessor::getInputChannelName (int channelIndex) const {
    return "input name"; // sampler->getInputChannelName(channelIndex);
}

const String PluginProcessor::getOutputChannelName (int channelIndex) const {
    return "out name"; // sampler->getOutputChannelName(channelIndex);
}

bool PluginProcessor::isInputChannelStereoPair  (int) const { return true; }
bool PluginProcessor::isOutputChannelStereoPair (int) const { return true; }

bool PluginProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PluginProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PluginProcessor::silenceInProducesSilenceOut() const {
    return false; // sampler->silenceInProducesSilenceOut();
}

double PluginProcessor::getTailLengthSeconds() const {
    return 0.0; // sampler->getTailLengthSeconds();
}

int PluginProcessor::getNumPrograms() {
    return 0;// sampler->getNumPrograms();
}

int PluginProcessor::getCurrentProgram() {
    return 0;// sampler->getCurrentProgram();
}

void PluginProcessor::setCurrentProgram (int index) {
    // return sampler->setCurrentProgram (index);
}

const String PluginProcessor::getProgramName (int index) {
    return ""; // sampler->getProgramName(index);
}

void PluginProcessor::changeProgramName (int index, const String& newName) {
    //sampler->changeProgramName (index, newName);
}

void PluginProcessor::prepareToPlay (double sampleRate, int blockSize)
{
    setPlayConfigDetails (0, 2, sampleRate, blockSize);
    ring = new RingBuffer(4096 * 3);
    module->instantiate (sampleRate);
    module->activate();
}

void PluginProcessor::releaseResources()
{
    module->deactivate();
    module->cleanup();
    ring = nullptr;
}

void PluginProcessor::processBlock(AudioSampleBuffer& audio, MidiBuffer& midi)
{
    module->process (audio, midi);
}

bool PluginProcessor::hasEditor() const { return true; }

AudioProcessorEditor* PluginProcessor::createEditor()
{
    Gui::PluginEditor* ed = new Gui::PluginEditor (this, *globals);
    return ed;
}

void PluginProcessor::getStateInformation (MemoryBlock& destData)
{
    InstrumentPtr sinst;
#if 0
    //FIXME:
    if (SamplerSynth* synth = sampler->currentSynth())
        sinst = synth->getInstrument();


    jassert (sinst != nullptr);
    if (ScopedXml xml = sinst->createXml())
    {
        copyXmlToBinary (*xml, destData);
        //Logger::writeToLog (instrument->node().toXmlString());
    }
#endif
}

void
PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
#if 0
    if (ScopedXml xml = getXmlFromBinary (data, sizeInBytes))
    {
        ValueTree data (ValueTree::fromXml (*xml));
        if (data.isValid())
        {
            instrument->setData (data);
            // FIXME: sampler->currentSynth()->setInstrument (instrument);
        }
    }
#endif
}

void PluginProcessor::writeToPort (uint32 portIndex, uint32 bufferSize, uint32 portProtocol, const void* buffer)
{
    if (nullptr == ring)
        ring = new RingBuffer (4096 * 3);

    const uint32 totalSize = bufferSize + sizeof(PortEvent);

    if (ring->canWrite (totalSize))
    {
        PortEvent ev;
        ev.index = portIndex;
        ev.protocol = portProtocol;
        ev.size = bufferSize;
        ev.time.frames = 0;
        ring->write (&ev, sizeof (PortEvent));
        // ring->write (buffer, bufferSize);
    }
}

}
