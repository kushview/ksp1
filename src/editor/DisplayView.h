#ifndef DISPLAYVIEW_H
#define DISPLAYVIEW_H

#include "editor/SamplerDisplay.h"

namespace KSP1 {
namespace Gui {

class DisplayView :  public Component
{
public:
    DisplayView();
    virtual ~DisplayView();

    void paint (Graphics&) override;
    void resized() override;

    SamplerDisplay& getDisplay() { return display; }

private:
    SamplerDisplay display;
};

}}

#endif // DISPLAYVIEW_H
