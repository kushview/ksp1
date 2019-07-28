
#pragma once

#include "PluginProcessor.h"

namespace KSP1 {

class PluginProcessor;
class PluginWorld;
class SamplerView;

class PluginEditor  : public AudioProcessorEditor,
                      public ChangeListener
{
public:
    PluginEditor (PluginProcessor* plug, PluginWorld& pw);
    ~PluginEditor();

    MidiKeyboardState& getMidiKeyboardState();

    void paint (Graphics& g) override;
    void resized() override;
    
    void changeListenerCallback (ChangeBroadcaster*) override;
    
private:
    PluginWorld& world;
    kv::LookAndFeel_KV1 style;
    std::unique_ptr<SamplerView> view;
};

}
