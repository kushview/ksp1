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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "FilterControls.h"

namespace KSP1 {

//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
FilterControls::FilterControls() {
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    filterTypes.reset (new ComboBox ("filter-types"));
    addAndMakeVisible (filterTypes.get());
    filterTypes->setTooltip (TRANS ("Filter Type"));
    filterTypes->setEditableText (false);
    filterTypes->setJustificationType (Justification::centredLeft);
    filterTypes->setTextWhenNothingSelected (String());
    filterTypes->setTextWhenNoChoicesAvailable (TRANS ("(no choices)"));
    filterTypes->addItem (TRANS ("High Pass"), 1);
    filterTypes->addItem (TRANS ("Low Pass"), 2);
    filterTypes->addListener (this);

    filterTypes->setBounds (24, 8, 127, 24);

    //[UserPreSize]
    //[/UserPreSize]

    setSize (177, 100);

    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

FilterControls::~FilterControls() {
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    filterTypes = nullptr;

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void FilterControls::paint (Graphics& g) {
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void FilterControls::resized() {
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void FilterControls::comboBoxChanged (ComboBox* comboBoxThatHasChanged) {
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == filterTypes.get()) {
        //[UserComboBoxCode_filterTypes] -- add your combo box handling code here..
        //[/UserComboBoxCode_filterTypes]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]

//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="FilterControls" template="../../jucer/Templates/GuiTemplate.cpp"
                 componentName="" parentClasses="public Component" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="1" initialWidth="177" initialHeight="100">
  <BACKGROUND backgroundColour="ffffff"/>
  <COMBOBOX name="filter-types" id="7ed090ac436aeef5" memberName="filterTypes"
            virtualName="" explicitFocusOrder="0" pos="24 8 127 24" tooltip="Filter Type"
            editable="0" layout="33" items="High Pass&#10;Low Pass" textWhenNonSelected=""
            textWhenNoItems="(no choices)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

} // namespace KSP1

//[EndFile] You can add extra defines here...
//[/EndFile]
