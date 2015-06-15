/*
    PluginProcessor.cpp - This file is part of KSP1

    Copyright (C) 2013 Kushview, LLC  All rights reserved.
      * Michael Fisher <mfisher@kushview.net>
*/

#include "../../lvtk/lvtk/plugin.hpp"
#include "engine/SamplerSynth.h"
#include "Locations.h"
#include "PluginEditor.h"
#include "PluginProcessor.h"


using namespace Element;

namespace KSP1
{    
    class PluginWorld
    {
    public:

        PluginWorld() { }
        
        void init ()
        {
            workThread = new WorkThread ("KSP1_Worker", 4096, 5);
            workThread->startThread();

            features.add (symbols.createMapFeature());
            features.add (symbols.createUnmapFeature());
            features.add (new LV2Worker());
        }
        
        void registerPlugin (PluginProcessor* plug)
        {
            if (instances.size() == 0)
                init();
            
            instances.addIfNotAlreadyThere (plug);
        }
        
        bool unregisterPlugin (PluginProcessor* plug)
        {
            instances.removeFirstMatchingValue (plug);
            return instances.size() == 0;
        }
        
        AudioProcessor* load (const String& uri)
        {
#if 0
            String e;
            Element::Processor* proc = nullptr;
            
            if (PluginDescription* desc = plugins().availablePlugins().getTypeForFile (uri))
                proc = plugins().createPlugin (*desc, e);
            else
                e = "plugin not available";
                        
            return proc;
#endif
            return nullptr;
        }
    
        Array<PluginProcessor*> instances;
        SymbolMap symbols;
        LV2FeatureArray features;
        ScopedPointer<WorkThread> workThread;
    };
   
    static ScopedPointer<PluginWorld> globals;
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    using namespace KSP1;

    if (!globals)
    {
        globals = new KSP1::PluginWorld();
        globals->init();
    }
    PluginProcessor* plugin = new PluginProcessor();
    return plugin;
}

namespace KSP1 {

PluginProcessor::PluginProcessor()
{
    jassert (lvtk::get_lv2_descriptors().size() == 1);
    LV2_Descriptor* descriptor = &lvtk::get_lv2_descriptors()[0];
    const LV2_Feature* feats[] = { nullptr };
    descriptor->instantiate (descriptor, 44100.0, "c:\\SDKs", globals->features);
    
#if 0
    useExternalData = false;
    if (! globals)
        globals = new KSP1::PluginWorld();

    globals->registerPlugin (this);
    instrument = new Instrument ("New Instrument");
    sampler = new SamplerProcessor (*globals);
    

    //FIXME:
    if (SamplerSynth* s = static_cast<SamplerSynth*> (sampler->currentSynth())) {
        s->setGain (1.0);
        s->setInstrument (instrument);
    } else {
        // need to have a synth to make sounds ..
        jassertfalse;
    }
#endif
}

PluginProcessor::~PluginProcessor()
{
#if 0
    if (sampler)
        sampler->releaseResources();

    sampler    = nullptr;
    instrument = nullptr;

    const bool shutdown = ! useExternalData ? globals->unregisterPlugin (this)
                                            : false;
    if (shutdown)
        globals = nullptr;
#endif
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

bool PluginProcessor::isInputChannelStereoPair  (int index) const { return true; }
bool PluginProcessor::isOutputChannelStereoPair (int index) const { return true; }

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
#if 0
    int32 outputs = getNumOutputChannels();
    if (outputs < 2)
    {
        setPlayConfigDetails (0, 2, sampleRate, blockSize);
        outputs = getNumOutputChannels();
    }

    sampler->prepareToPlay (sampleRate, blockSize);
#endif
}

void PluginProcessor::releaseResources()
{
    //sampler->releaseResources();
}

void PluginProcessor::processBlock (AudioSampleBuffer& audio, MidiBuffer& midi)
{
    //sampler->processBlock (audio, midi);
}

bool PluginProcessor::hasEditor() const { return true; }

AudioProcessorEditor* PluginProcessor::createEditor()
{
    Gui::PluginEditor* ed = new Gui::PluginEditor (this);
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

}
