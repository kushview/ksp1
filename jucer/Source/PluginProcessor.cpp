/*
    PluginProcessor.cpp - This file is part of KSP1

    Copyright (C) 2015 Kushview, LLC  All rights reserved.
      * Michael Fisher <mfisher@kushview.net>
*/

#include "KSP1.h"
#include "engine/SamplerSynth.h"
#include "DataPath.h"
#include "PluginEditor.h"
#include "PluginProcessor.h"

namespace KSP1 {
   
    PluginWorld::PluginWorld()
    {

    }
    
    PluginWorld::~PluginWorld()
    {
        workThread = nullptr;
    }

    void PluginWorld::registerPlugin (PluginProcessor* plug)
    {
        if (instances.size() == 0)
            init();

        instances.addIfNotAlreadyThere (plug);
    }

    bool PluginWorld::unregisterPlugin (PluginProcessor* plug)
    {
        instances.removeFirstMatchingValue (plug);
        return instances.size() == 0;
    }
        
    AudioProcessor* PluginWorld::load (const String& uri)
    {
        return nullptr;
    }
    
    kv::WorkThread& PluginWorld::getWorkThread()
    {
        jassert (workThread);
        return *workThread;
    }

    void PluginWorld::init()
    {
        workThread = new kv::WorkThread ("KSP1_Worker", 4096, 5);
        workThread->startThread();
        factoryInstruments.clearQuick();
        DataPath::factoryContentPath().getChildFile("Instruments")
            .findChildFiles (factoryInstruments, File::findFiles, true, "*.xml");
    }

    static ScopedPointer<PluginWorld> globals;
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return KSP1::PluginProcessor::create();
}

namespace KSP1 {

PluginProcessor::PluginProcessor()
{
    currentProgram = 0;
    globals->registerPlugin (this);
}

PluginProcessor::~PluginProcessor()
{
    if (editors.size() > 0)
    {
        jassertfalse;
        stopTimer();
        for (auto* e : editors)
            delete e;
        editors.clear();
    }

    if (globals->unregisterPlugin (this))
        globals = nullptr;
}

PluginProcessor* PluginProcessor::create()
{
    if (! globals)
        globals = new PluginWorld();

    PluginProcessor* plugin = new PluginProcessor();
    return plugin;
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
#ifdef JucePlugin_Name
    return JucePlugin_Name;
#else
    return "KSP1";
#endif
}
int PluginProcessor::getNumParameters() { return 0; }
float PluginProcessor::getParameter (int index) { return 0.0f; }
void PluginProcessor::setParameter (int index, float newValue) { }
const String PluginProcessor::getParameterName (int index) { return String(); }
const String PluginProcessor::getParameterText (int index) { return String(); }
const String PluginProcessor::getInputChannelName (int channelIndex) const { return String(); }

const String PluginProcessor::getOutputChannelName (int channelIndex) const
{
    return String ("Main ") + String (channelIndex == 0 ? "Left" : "Right");
}

bool PluginProcessor::isInputChannelStereoPair  (int) const { return false; }
bool PluginProcessor::isOutputChannelStereoPair (int) const { return true; }
bool PluginProcessor::acceptsMidi() const { return true; }
bool PluginProcessor::producesMidi() const { return true; }
bool PluginProcessor::silenceInProducesSilenceOut() const { return false; }
double PluginProcessor::getTailLengthSeconds() const { return 0.0; }
int PluginProcessor::getNumPrograms() { return globals->factoryInstruments.size(); }
int PluginProcessor::getCurrentProgram() { return currentProgram; }

void PluginProcessor::setCurrentProgram (int index)
{
    if (isPositiveAndBelow (index, globals->factoryInstruments.size())) {
    }
}

const String PluginProcessor::getProgramName (int index)
{
    return isPositiveAndBelow(index, globals->factoryInstruments.size())
        ? globals->factoryInstruments[index].getFileNameWithoutExtension() : "Program not available";
}

void PluginProcessor::changeProgramName (int /*index*/, const String& /*newName*/) { }

void PluginProcessor::prepareToPlay (double sampleRate, int blockSize)
{
    setPlayConfigDetails (0, 2, sampleRate, blockSize);
    ring = new kv::RingBuffer (4096 * 3);
    uiRing = new kv::RingBuffer (4096 * 3);
    block.allocate (1024, true);
}

void PluginProcessor::releaseResources()
{
    block.free();
    ring = nullptr;
    uiRing = nullptr;
}

void PluginProcessor::processBlock (AudioSampleBuffer& audio, MidiBuffer& midi)
{
    audio.clear();
    midi.clear();
   #if 0
    // save for reference
    if (ring->canRead (sizeof (uint32)))
    {
        uint32 totalSize = 0;
        ring->peak (&totalSize, sizeof (uint32));
        bool readEvents = ring->canRead (totalSize);
        kv::PortEvent ev;
        
        while (readEvents)
        {
            zerostruct (ev);
            ring->read (&totalSize, sizeof (uint32));
            ring->read (&ev, sizeof (PortEvent));
            jassert(totalSize == sizeof(uint32) + ev.size + sizeof(PortEvent));
            ring->read (block, ev.size);
            
            if (ev.index == Port::AtomInput)
            {
                const lvtk::Atom atom (block.getData());
                atomIn->addEvent (ev.time.frames,
                                  atom.size(), atom.type(),
                                  (uint8*) atom.body());
            }
            
            if (ring->canRead (sizeof (uint32)))
            {
                ring->peak (&totalSize, sizeof (uint32));
                readEvents = ring->canRead (totalSize);
            }
            else
            {
                readEvents = false;
            }
        }
    }
    
    if (midi.getNumEvents() > 0)
    {
        MidiBuffer::Iterator iter (midi);
        const uint8* data = nullptr; int bytes = 0, frame = 0;
        while (iter.getNextEvent (data, bytes, frame))
            atomIn->addEvent (frame, static_cast<uint32> (bytes),
                              uris->midi_MidiEvent, data);
        midi.clear();
    }
    
    atomOut->reset (true);
    module->connectPort (Port::AtomInput,  atomIn->getPortData());
    module->connectPort (Port::AtomOutput, atomOut->getPortData());
    module->connectPort (Port::MainLeft,   audio.getWritePointer (0, 0));
    module->connectPort (Port::MainRight,  audio.getWritePointer (1, 0));
    module->run (audio.getNumSamples());
    
    LV2_ATOM_SEQUENCE_FOREACH ((LV2_Atom_Sequence*) atomOut->getPortData(), ev)
    {
        uint32 totalSize = lv2_atom_total_size (&ev->body);
        if (uiRing->canWrite (totalSize))
            uiRing->write (&ev->body, totalSize);
    }
   #endif
}

bool PluginProcessor::hasEditor() const { return true; }

AudioProcessorEditor* PluginProcessor::createEditor()
{
    PluginEditor* ed = new PluginEditor (this, *globals);
    editors.add (ed);
    
    if (! isTimerRunning())
        startTimer (52);
    
    return ed;
}

void PluginProcessor::unregisterEditor (PluginEditor* ed)
{
    jassert (editors.contains (ed));
    editors.removeFirstMatchingValue (ed);
    
    if (editors.size() <= 0 && isTimerRunning())
        stopTimer();
}

void PluginProcessor::getStateInformation (MemoryBlock& destData)
{
    SamplerSynth* sampler = nullptr;
    if (! sampler)
        return;
    
    var data;
    sampler->getNestedVariant (data);
    const String json (JSON::toString (data, true));
    destData.append (json.toRawUTF8(), strlen(json.toRawUTF8()));
}

void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    SamplerSynth* sampler = nullptr;
    if (! sampler)
        return;
    
    const String json (CharPointer_UTF8 ((const char*) data),
                       (size_t) sizeInBytes);
    
    if (! sampler->loadJSON (json))
    {
        DBG("Failed loading JSON data");
    }
}

void PluginProcessor::writeToPort (uint32 portIndex, uint32 bufferSize, uint32 portProtocol, const void* buffer)
{
   #if 0
    // save for ref.
    if (nullptr == ring)
        ring = new kv::RingBuffer (4096 * 3);

    const uint32 totalSize = sizeof(uint32) + bufferSize + sizeof (PortEvent);

    if (ring->canWrite (totalSize))
    {
        PortEvent ev;
        ev.index = portIndex;
        ev.protocol = portProtocol;
        ev.size = bufferSize;
        ev.time.frames = 0;
        ring->write (&totalSize, sizeof (uint32));
        ring->write (&ev, sizeof (PortEvent));
        ring->write (buffer, bufferSize);
    }
    else
    {
        
    }
   #endif
}

void PluginProcessor::timerCallback()
{
    if (! uiRing)
        return;
    
   #if 0
    // save for ref
    HeapBlock<uint8> block; block.allocate (1024, true);
    LV2_Atom* atom = (LV2_Atom*) block.getData();
    while (uiRing->getReadSpace() > sizeof (LV2_Atom))
    {
        if (sizeof (LV2_Atom) != uiRing->peak (atom, sizeof (LV2_Atom)))
            break;
        
        const uint32 totalSize (lv2_atom_total_size (atom));
        
        if (! uiRing->canRead (totalSize))
            break;
        
        if (totalSize == uiRing->read (atom, lv2_atom_total_size (atom)))
        {
            for (PluginEditor* editor : editors)
                editor->receiveNotification (atom);
        }
        else
        {
            DBG ("Error reading plugin notification");
        }
    }
   #endif
}

}
