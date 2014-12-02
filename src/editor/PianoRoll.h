/*
    Copyright 2013  Kushview, LLC.  All rights reserved.
    This is an automatically generated GUI class created by the Introjucer!
*/

#ifndef __JUCE_HEADER_8334ACCEEC97308__
#define __JUCE_HEADER_8334ACCEEC97308__

//[Headers]
#include "Screens.h"
//[/Headers]



namespace KSP1 {
namespace Gui {

/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class PianoRollScreen  : public Screen,
                         public Timer,
                         public ButtonListener
{
public:
    PianoRollScreen (SamplerDisplay& owner);
    ~PianoRollScreen();

    //[UserMethods]
    void keySelectedEvent (const KeyItem&);
    void onDisplayUpdate();
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);



private:
    //[UserVariables]
    int lastNote;
    void timerCallback();
    void updateComponents();
    //[/UserVariables]

    ScopedPointer<TextButton> buttonAddSample;
    ScopedPointer<TextButton> buttonRemoveLayer;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PianoRollScreen)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

}} /* namespace KSP1::Gui */

#endif   // __JUCE_HEADER_8334ACCEEC97308__
