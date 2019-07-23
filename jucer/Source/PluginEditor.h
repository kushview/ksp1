
#pragma once

#include "PluginProcessor.h"

namespace KSP1 {

class PluginProcessor;
class PluginWorld;

class PluginEditor  : public AudioProcessorEditor
{
public:
    PluginEditor (PluginProcessor* plug, PluginWorld& pw);
    ~PluginEditor();

    void paint (Graphics& g) override;
    void resized() override;
    
private:
    PluginWorld& world;
    LookAndFeel_V3 style;
};

}
