
#ifndef KSP1_PLUGIN_EDITOR_H
#define KSP1_PLUGIN_EDITOR_H

#include "PluginProcessor.h"
#include "Instrument.h"

namespace KSP1 {
namespace Gui {

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
    Element::Style style;
    class Module; ScopedPointer<Module> module;
};

}}

#endif  // KSP1_PLUGIN_EDITOR_H
