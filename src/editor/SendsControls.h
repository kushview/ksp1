/*
    Copyright 2014, Kushview, LLC
    This is an automatically generated GUI class created by the Introjucer!
*/

#ifndef __JUCE_HEADER_617BEDA8D8339FDE__
#define __JUCE_HEADER_617BEDA8D8339FDE__

//[Headers]
#include "KSP1.h"

namespace KSP1 {
namespace Gui {
//[/Headers]



namespace KSP1 {
namespace Gui {

/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class SendsControls  : public Component,
                       public SliderListener
{
public:
    SendsControls ();
    ~SendsControls();

    //[UserMethods]
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);



private:
    //[UserVariables]
    //[/UserVariables]

    ScopedPointer<Slider> slider;
    ScopedPointer<Slider> slider2;
    ScopedPointer<Label> label;
    ScopedPointer<Label> label2;
    ScopedPointer<GroupComponent> groupComponent;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SendsControls)
};

//[EndFile] You can add extra defines here...
}}
//[/EndFile]

}} /* namespace BTV::Gui */

#endif   // __JUCE_HEADER_617BEDA8D8339FDE__
