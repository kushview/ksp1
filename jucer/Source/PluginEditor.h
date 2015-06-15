
#ifndef BTSP1_EDITOR_H
#define BTSP1_EDITOR_H

#include "PluginProcessor.h"

namespace KSP1 {
namespace Gui {

class PluginEditor  : public AudioProcessorEditor
{
public:
    PluginEditor (PluginProcessor* plugin);
    ~PluginEditor();

    void setInstrument (InstrumentPtr i);

    void paint (Graphics& g) override;
    void resized() override;
    
private:
    Element::Style style;
};

}}

#endif  // BTSP1_EDITOR_H
