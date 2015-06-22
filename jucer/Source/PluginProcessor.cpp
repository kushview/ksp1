/*
    PluginProcessor.cpp - This file is part of KSP1

    Copyright (C) 2013 Kushview, LLC  All rights reserved.
      * Michael Fisher <mfisher@kushview.net>
*/

#include "../../lvtk/lvtk/plugin.hpp"
#include "../../lvtk/lvtk/ui.hpp"
#include "KSP1.h"
#include "engine/LV2Plugin.h"
#include "engine/SamplerSynth.h"
#include "Locations.h"
#include "PluginEditor.h"
#include "PluginProcessor.h"
#include "Ports.h"

namespace KSP1
{
    ScopedPointer<Element::URIs> uris;
    
    PluginWorld::PluginWorld()
        : symbols()
    {
    }
    
    PluginWorld::~PluginWorld()
    {
        symbols.clear();
        workThread = nullptr;
    }

    LV2_URID PluginWorld::map (const char* uri)
    {
        return symbols.map (uri);
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
        factoryInstruments.clearQuick();
        DataPath::factoryContentPath().getChildFile("Instruments")
            .findChildFiles (factoryInstruments, File::findFiles, true, "*.xml");
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
            worker = nullptr;
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
                worker = nullptr;
                features = nullptr;
                jassert (nullptr == handle && nullptr == workerInterface);
            }
            
            if (nullptr == handle)
            {
                jassert (worker == nullptr);
                features = new LV2FeatureArray();
                features->add (globals->createMapFeature(), false);
                features->add (globals->createUnmapFeature());
                worker = new LV2Worker (globals->getWorkThread(), 2048);
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
        
        void run (int frames)
        {
            descriptor->run (handle, static_cast<uint32> (frames));
            worker->processWorkResponses();
            worker->endRun();
        }
        
        void connectPort (uint32 index, void* data) {
            descriptor->connect_port (handle, index, data);
        }

        LV2Plugin* getPlugin() const
        {
            return static_cast<KSP1::LV2Plugin*> (handle);
        }
        
        SamplerSynth* getSynth()
        {
            if (KSP1::LV2Plugin* plugin = getPlugin())
                return plugin->get_sampler_synth();
            return nullptr;
        }
    private:
        const LV2_Descriptor* descriptor;
        const LV2UI_Descriptor* uiDescriptor;
        LV2_Handle handle;
        LV2UI_Handle uiHandle;
        LV2_Worker_Interface* workerInterface;
        ScopedPointer<LV2FeatureArray> features;
        double currentSampleRate;
        LV2Worker* worker;
    };
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    using namespace KSP1;

    if (! globals)
    {
        globals = new PluginWorld();
        ScopedPointer<LV2Feature> feat (globals->createMapFeature());
        uris = new Element::URIs ((LV2_URID_Map*) feat->getFeature()->data);
    }
    
    PluginProcessor* plugin = new PluginProcessor();
    return plugin;
}

namespace KSP1 {

PluginProcessor::PluginProcessor()
{
    currentProgram = 0;
    jassert (lvtk::get_lv2_descriptors().size() == 1);
    jassert (lvtk::get_lv2g2g_descriptors().size() == 1);
    module = new PluginModule();
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
    
    module->deactivate();
    module->cleanup();
    module = nullptr;

    if (globals->unregisterPlugin (this))
        globals = nullptr;
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

bool PluginProcessor::silenceInProducesSilenceOut() const { return false; }
double PluginProcessor::getTailLengthSeconds() const { return 0.0; }
int PluginProcessor::getNumPrograms() { return globals->factoryInstruments.size(); }
int PluginProcessor::getCurrentProgram() { return currentProgram; }

void PluginProcessor::setCurrentProgram (int index)
{
    if (isPositiveAndBelow (index, globals->factoryInstruments.size())) {
        if (LV2Plugin* plugin = module->getPlugin()) {
            if (SamplerSynth* synth = module->getSynth()) {
                currentProgram = index;
                const File file (globals->factoryInstruments.getUnchecked (index));
                if (file.existsAsFile()) {
                    synth->loadFile (globals->factoryInstruments.getUnchecked (index));
                    plugin->trigger_restored();
                }
            }
        }
    }
}

const String PluginProcessor::getProgramName (int index) {
    return isPositiveAndBelow(index, globals->factoryInstruments.size())
        ? globals->factoryInstruments[index].getFileNameWithoutExtension() : "Program not available";
}

void PluginProcessor::changeProgramName (int /*index*/, const String& /*newName*/) { }

void PluginProcessor::prepareToPlay (double sampleRate, int blockSize)
{
    setPlayConfigDetails (0, 2, sampleRate, blockSize);
    ring = new RingBuffer (4096 * 3);
    uiRing = new RingBuffer (4096 * 3);
    block.allocate (1024, true);
    atomIn  = new PortBuffer (uris, uris->atom_Sequence, 4096);
    atomOut = new PortBuffer (uris, uris->atom_Sequence, 4096);
    module->instantiate (sampleRate);
    module->activate();
}

void PluginProcessor::releaseResources()
{
    module->deactivate();
    module->cleanup();
    block.free();
    atomIn = nullptr;
    ring = nullptr;
    uiRing = nullptr;
}

void PluginProcessor::processBlock (AudioSampleBuffer& audio, MidiBuffer& midi)
{
    atomIn->clear();
    if (ring->canRead (sizeof (uint32)))
    {
        uint32 totalSize = 0;
        ring->peak (&totalSize, sizeof (uint32));
        bool readEvents = ring->canRead (totalSize);
        PortEvent ev;
        
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
}

bool PluginProcessor::hasEditor() const { return true; }

AudioProcessorEditor* PluginProcessor::createEditor()
{
    Gui::PluginEditor* ed = new Gui::PluginEditor (this, *globals);
    editors.add (ed);
    
    if (! isTimerRunning())
        startTimer (52);
    
    return ed;
}



void PluginProcessor::unregisterEditor (Gui::PluginEditor* ed)
{
    jassert (editors.contains (ed));
    editors.removeFirstMatchingValue (ed);
    
    if (editors.size() <= 0 && isTimerRunning())
        stopTimer();
}

void PluginProcessor::getStateInformation (MemoryBlock& destData)
{
    SamplerSynth* sampler = module->getSynth();
    if (! sampler)
        return;
    
    var data;
    sampler->getNestedVariant (data);
    const String json (JSON::toString (data, true));
    destData.append (json.toRawUTF8(), strlen(json.toRawUTF8()));
}

void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    LV2Plugin* plugin = module->getPlugin();
    SamplerSynth* sampler = module->getSynth();
    if (! sampler || ! plugin)
        return;
    
    const String json (CharPointer_UTF8 ((const char*) data),
                       (size_t) sizeInBytes);
    
    if (! sampler->loadJSON (json))
    {
        DBG("Failed loading JSON data");
    }
    
    plugin->trigger_restored();
}

void PluginProcessor::writeToPort (uint32 portIndex, uint32 bufferSize, uint32 portProtocol, const void* buffer)
{
    if (nullptr == ring)
        ring = new RingBuffer (4096 * 3);

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
}

void PluginProcessor::timerCallback()
{
    if (! uiRing)
        return;
    
    HeapBlock<uint8> block; block.allocate (1024, true);
    LV2_Atom* atom = (LV2_Atom*) block.getData();
    while (uiRing->getReadSpace() > sizeof (LV2_Atom))
    {
        if (sizeof (LV2_Atom) != uiRing->peak (atom, sizeof(LV2_Atom)))
            break;
        
        const uint32 totalSize (lv2_atom_total_size (atom));
        
        if (! uiRing->canRead (totalSize))
            break;
        
        if (totalSize == uiRing->read (atom, lv2_atom_total_size (atom)))
        {
            for (Gui::PluginEditor* editor : editors)
                editor->receiveNotification (atom);
                    
        }
        else
        {
            DBG ("Error reading plugin notification");
        }
    }
}

}
