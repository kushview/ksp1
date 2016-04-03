
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
    LV2UI_Handle getHandle() const;
    void receiveNotification (const LV2_Atom* atom);
    
private:
    PluginWorld& world;
    Element::LookAndFeel_E1 style;
    class Module; ScopedPointer<Module> module;
};

}}

#endif  // KSP1_PLUGIN_EDITOR_H
