
#pragma once

#include "PluginProcessor.h"

namespace KSP1 {

class PluginProcessor;
class PluginWorld;
class SamplerView;

class PluginEditor  : public AudioProcessorEditor
{
public:
    PluginEditor (PluginProcessor* plug, PluginWorld& pw);
    ~PluginEditor();

    void paint (Graphics& g) override;
    void resized() override;
    
private:
    PluginWorld& world;
    kv::LookAndFeel_KV1 style;
    std::unique_ptr<SamplerView> view;
};

}
