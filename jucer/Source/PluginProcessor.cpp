/*
    PluginProcessor.cpp - This file is part of KSP1

    Copyright (C) 2015 Kushview, LLC  All rights reserved.
      * Michael Fisher <mfisher@kushview.net>
*/

#include "KSP1.h"
#include "engine/SamplerSynth.h"
#include "engine/SamplerSounds.h"

#include "DataPath.h"
#include "Instrument.h"
#include "PluginEditor.h"
#include "PluginProcessor.h"

namespace KSP1 {
   
PluginWorld::PluginWorld() { }

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

//=============================================================================
PluginProcessor::PluginProcessor()
    : AudioProcessor (BusesProperties()
        .withOutput ("Main",  AudioChannelSet::stereo(), true)
        .withOutput ("Aux 1", AudioChannelSet::stereo(), true)
        .withOutput ("Aux 2", AudioChannelSet::stereo(), true)
        .withOutput ("Aux 3", AudioChannelSet::stereo(), true)
        .withOutput ("Aux 4", AudioChannelSet::stereo(), true))
{
    currentProgram = 0;
    globals->registerPlugin (this);

    synth.reset (SamplerSynth::create());
    instrument = new Instrument ("Instrument");
    data = instrument->getValueTree();
    data.addListener (this);
}

PluginProcessor::~PluginProcessor()
{
    data.removeListener (this);
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

const String PluginProcessor::getName() const
{
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

void removePropertyRecursive (ValueTree data, const Identifier& property)
{
    data.removeProperty (property, nullptr);
    for (int i = 0; i < data.getNumChildren(); ++i)
        removePropertyRecursive (data.getChild (i), property);
}

void PluginProcessor::getStateInformation (MemoryBlock& destData)
{
    auto saveData = data.createCopy();
    removePropertyRecursive (saveData, "object");
    MemoryOutputStream mo (destData, false);
    {
        GZIPCompressorOutputStream gzip (mo);
        saveData.writeToStream (gzip);
    }
}

void PluginProcessor::setStateInformation (const void* stateData, int sizeInBytes)
{
    data = ValueTree::readFromGZIPData (stateData, (size_t) sizeInBytes);
    instrument = new Instrument (data);
    sendChangeMessage();
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

void PluginProcessor::valueTreePropertyChanged (ValueTree& tree, const Identifier& property)
{
    if (tree.hasType (Tags::key))
    {
        DBG("key: " << property.toString());
    }
}

void PluginProcessor::valueTreeChildAdded (ValueTree& parent, ValueTree& child)
{
    if (parent == data && child.hasType (Tags::key))
    {
        KeyItem soundItem (child);
        auto* const sound = new SamplerSound (soundItem.getNote());
        sound->setLength ((int) soundItem.getProperty (Tags::length));
        synth->insertSound (sound);
        soundItem.setProperty (Tags::id, sound->getObjectId())
                 .setProperty ("object", sound);
    }
}

void PluginProcessor::valueTreeChildRemoved (ValueTree& parent, ValueTree& child, int childIndex)
{
    if (parent == data && child.hasType (Tags::key))
    {
        if (SamplerSoundPtr ptr = dynamic_cast<SamplerSound*> (child.getProperty ("object").getObject()))
        {
            for (int i = 0; i < synth->getNumSounds(); ++i)
                if (ptr.get() == synth->getSound(i).get())
                    { synth->removeSound (i); break; }
            jassert (synth->getNumSounds() == instrument->getNumChildren());
        }
    }
}

void PluginProcessor::valueTreeChildOrderChanged (ValueTree& parent, int oldIndex, int newIndex) { ignoreUnused (parent, oldIndex, newIndex); }
void PluginProcessor::valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged) {}

}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return KSP1::PluginProcessor::create();
}
