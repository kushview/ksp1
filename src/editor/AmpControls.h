/*
    Copyright 2013  Kushview, LLC.  All rights reserved.
    This is an automatically generated GUI class created by the Introjucer!
*/

#ifndef __JUCE_HEADER_CE5B3F3E59E092BC__
#define __JUCE_HEADER_CE5B3F3E59E092BC__

//[Headers]
#include "KSP1.h"

//[/Headers]



namespace KSP1 {
namespace Gui {

/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class AmpControls  : public Component,
                     public SliderListener
{
public:
    AmpControls ();
    ~AmpControls();

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
    ScopedPointer<Slider> slider3;
    ScopedPointer<Slider> slider4;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmpControls)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

}} /* namespace BTV::Gui */

#endif   // __JUCE_HEADER_CE5B3F3E59E092BC__
