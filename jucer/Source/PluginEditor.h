
#pragma once

#include "PluginProcessor.h"
#include "Instrument.h"

namespace KSP1 {

class PluginEditor  : public AudioProcessorEditor
{
public:
    PluginEditor (PluginProcessor* plug, PluginWorld& pw);
    ~PluginEditor();

    void setInstrument (InstrumentPtr i);

    void paint (Graphics& g) override;
    void resized() override;
    
private:
    PluginWorld& world;
    LookAndFeel style;
};

}
