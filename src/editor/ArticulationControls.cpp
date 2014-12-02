/*
    This file is part of KSP1
    Copyright (C) 2014  Kushview, LLC. All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    GUI class created by the Introjucer!
*/

//[Headers]
#include "JuceHeader.h"
#include "../Articulation.h"
#include "../Types.h"
//[/Headers]

#include "ArticulationControls.h"


namespace KSP1 {
namespace Gui {

//[MiscUserDefs]
//[/MiscUserDefs]

ArticulationControls::ArticulationControls ()
{
    setName ("ArticulationControls");
    addAndMakeVisible (articulationType = new ComboBox ("ArticulationType"));
    articulationType->setEditableText (false);
    articulationType->setJustificationType (Justification::centredLeft);
    articulationType->setTextWhenNothingSelected (TRANS("Articulation"));
    articulationType->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    articulationType->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor]
    for (int i = 0; i < StrokeType::numTypes; ++i) {
        StrokeType type (i);
        articulationType->addItem (type.getName(), i + 1);
    }
    //[/Constructor]
}

ArticulationControls::~ArticulationControls()
{
    //[Destructor_pre]
    //[/Destructor_pre]

    articulationType = nullptr;


    //[Destructor]
    //[/Destructor]
}

void ArticulationControls::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void ArticulationControls::resized()
{
    articulationType->setBounds (0, 0, proportionOfWidth (1.0000f), proportionOfHeight (1.0000f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void ArticulationControls::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == articulationType)
    {
        //[UserComboBoxCode_articulationType] -- add your combo box handling code here..
        DBG ("Art: " << StrokeType (articulationType->getSelectedId() - 1).getName());
        //[/UserComboBoxCode_articulationType]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode]

void
ArticulationControls::setModel (const KSP1::Articulation& model)
{
    ValueTree state (model.node());

    Value& artVal = articulationType->getSelectedIdAsValue();
    artVal.referTo (state.getPropertyAsValue (Slugs::type, nullptr));
}

//[/MiscUserCode]


#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
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


//[EndFile]
//[/EndFile]

}} /* namespace KSP1::Gui */
