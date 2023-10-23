/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.3

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "Screens.h"
//[/Headers]

namespace KSP1 {

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class PianoRollScreen : public Screen,
                        public Timer,
                        public Button::Listener {
public:
    //==============================================================================
    PianoRollScreen (SamplerDisplay& owner);
    ~PianoRollScreen();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void keySelectedEvent (const KeyItem&);
    void onDisplayUpdate();
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    int lastNote;
    void timerCallback();
    void updateComponents();
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<TextButton> buttonAddSample;
    std::unique_ptr<TextButton> buttonRemoveLayer;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PianoRollScreen)
};

} // namespace KSP1

//[EndFile] You can add extra defines here...
//[/EndFile]
