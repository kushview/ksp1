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
#include "KSP1.h"


namespace KSP1 {
    class Articulation;
}
//[/Headers]



namespace KSP1 {

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class ArticulationControls  : public Component,
                              public ComboBox::Listener
{
public:
    //==============================================================================
    ArticulationControls ();
    ~ArticulationControls();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void setModel (const KSP1::Articulation& model);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<ComboBox> articulationType;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArticulationControls)
};

} /* KSP1 */

//[EndFile] You can add extra defines here...
//[/EndFile]

