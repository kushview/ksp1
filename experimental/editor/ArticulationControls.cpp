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
#include "JuceHeader.h"
#include "../Articulation.h"
#include "../Types.h"
//[/Headers]

#include "ArticulationControls.h"


namespace KSP1 {

//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
ArticulationControls::ArticulationControls ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    setName ("ArticulationControls");
    articulationType.reset (new ComboBox ("ArticulationType"));
    addAndMakeVisible (articulationType.get());
    articulationType->setEditableText (false);
    articulationType->setJustificationType (Justification::centredLeft);
    articulationType->setTextWhenNothingSelected (TRANS("Articulation"));
    articulationType->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    articulationType->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    for (int i = 0; i < StrokeType::numTypes; ++i) {
        StrokeType type (i);
        articulationType->addItem (type.getName(), i + 1);
    }
    //[/Constructor]
}

ArticulationControls::~ArticulationControls()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    articulationType = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void ArticulationControls::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void ArticulationControls::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    articulationType->setBounds (0, 0, proportionOfWidth (1.0000f), proportionOfHeight (1.0000f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void ArticulationControls::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == articulationType.get())
    {
        //[UserComboBoxCode_articulationType] -- add your combo box handling code here..
        //[/UserComboBoxCode_articulationType]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void
ArticulationControls::setModel (const KSP1::Articulation& model)
{
    ValueTree state (model.node());

    Value& artVal = articulationType->getSelectedIdAsValue();
    artVal.referTo (state.getPropertyAsValue (Tags::type, nullptr));
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ArticulationControls" template="../../jucer/Templates/GuiTemplate.cpp"
                 componentName="ArticulationControls" parentClasses="public Component"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="600"
                 initialHeight="400">
  <BACKGROUND backgroundColour="ffffff"/>
  <COMBOBOX name="ArticulationType" id="36c84eeb56136843" memberName="articulationType"
            virtualName="" explicitFocusOrder="0" pos="0 0 100% 100%" editable="0"
            layout="33" items="" textWhenNonSelected="Articulation" textWhenNoItems="(no choices)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif



} /* KSP1 */

//[EndFile] You can add extra defines here...
//[/EndFile]

