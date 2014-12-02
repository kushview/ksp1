/*
    Copyright 2014, Kushview, LLC
    This is an automatically generated GUI class created by the Introjucer!
*/

//[Headers]
//[/Headers]

#include "FilterControls.h"


namespace KSP1 {
namespace Gui {

//[MiscUserDefs]
//[/MiscUserDefs]

FilterControls::FilterControls ()
{
    addAndMakeVisible (filterTypes = new ComboBox ("filter-types"));
    filterTypes->setTooltip (TRANS("Filter Type"));
    filterTypes->setEditableText (false);
    filterTypes->setJustificationType (Justification::centredLeft);
    filterTypes->setTextWhenNothingSelected (String::empty);
    filterTypes->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    filterTypes->addItem (TRANS("High Pass"), 1);
    filterTypes->addItem (TRANS("Low Pass"), 2);
    filterTypes->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (177, 100);


    //[Constructor]
    //[/Constructor]
}

FilterControls::~FilterControls()
{
    //[Destructor_pre]
    //[/Destructor_pre]

    filterTypes = nullptr;


    //[Destructor]
    //[/Destructor]
}

void FilterControls::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void FilterControls::resized()
{
    filterTypes->setBounds (24, 8, 127, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void FilterControls::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == filterTypes)
    {
        //[UserComboBoxCode_filterTypes] -- add your combo box handling code here..
        //[/UserComboBoxCode_filterTypes]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode]
//[/MiscUserCode]


#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
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


//[EndFile]
//[/EndFile]

}} /* namespace KSP1::Gui */
