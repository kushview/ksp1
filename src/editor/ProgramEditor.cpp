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
#include "editor/ProgramsListBox.h"
//[/Headers]

#include "ProgramEditor.h"


namespace KSP1 {
namespace Gui {

//[MiscUserDefs]
//[/MiscUserDefs]

ProgramEditorView::ProgramEditorView (Controller& ctrl)
    : controller (ctrl)
{
    addAndMakeVisible (bankList1 = new KSP1::Gui::BankList (world.programs()));
    bankList1->setName ("bank-list-1");

    addAndMakeVisible (programList = new ProgramsListBox (world.programs()));
    programList->setName ("program-list");

    addAndMakeVisible (banklist2 = new KSP1::Gui::BankList (world.programs()));
    banklist2->setName ("bank-list-2");


    //[UserPreSize]
    //[/UserPreSize]

    setSize (584, 350);


    //[Constructor]
    //[/Constructor]
}

ProgramEditorView::~ProgramEditorView()
{
    //[Destructor_pre]
    //[/Destructor_pre]

    bankList1 = nullptr;
    programList = nullptr;
    banklist2 = nullptr;


    //[Destructor]
    //[/Destructor]
}

void ProgramEditorView::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void ProgramEditorView::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    bankList1->setBounds (4, 5, 196, getHeight() - 276);
    programList->setBounds (208, 8, getWidth() - 214, getHeight() - 15);
    banklist2->setBounds (6, getHeight() - 263, 192, 256);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode]
//[/MiscUserCode]


#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ProgramEditorView" template="../../jucer/Templates/GuiTemplate.cpp"
                 componentName="" parentClasses="public KSP1::Gui::Panel" constructorParams="Controller&amp; ctrl"
                 variableInitialisers="controller (ctrl)" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="584"
                 initialHeight="350">
  <BACKGROUND backgroundColour="ffffffff"/>
  <GENERICCOMPONENT name="bank-list-1" id="c29d16f4dd6dc025" memberName="bankList1"
                    virtualName="" explicitFocusOrder="0" pos="4 5 196 276M" class="KSP1::Gui::BankList"
                    params="world.programs()"/>
  <GENERICCOMPONENT name="program-list" id="60f8a72dfb9daaf5" memberName="programList"
                    virtualName="" explicitFocusOrder="0" pos="208 8 214M 15M" class="ProgramsListBox"
                    params="world.programs()"/>
  <GENERICCOMPONENT name="bank-list-2" id="ed7855770431843d" memberName="banklist2"
                    virtualName="" explicitFocusOrder="0" pos="6 263R 192 256" class="KSP1::Gui::BankList"
                    params="world.programs()"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile]
//[/EndFile]

}} /* namespace KSP1::Gui */
