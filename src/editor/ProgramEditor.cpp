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
#include "editor/ProgramsListBox.h"
//[/Headers]

#include "ProgramEditor.h"


namespace KSP1 {

//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
ProgramEditorView::ProgramEditorView (Controller& ctrl)
    : controller (ctrl)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    bankList1.reset (new KSP1::Gui::BankList (world.programs()));
    addAndMakeVisible (bankList1.get());
    bankList1->setName ("bank-list-1");

    programList.reset (new ProgramsListBox (world.programs()));
    addAndMakeVisible (programList.get());
    programList->setName ("program-list");

    banklist2.reset (new KSP1::Gui::BankList (world.programs()));
    addAndMakeVisible (banklist2.get());
    banklist2->setName ("bank-list-2");


    //[UserPreSize]
    //[/UserPreSize]

    setSize (584, 350);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

ProgramEditorView::~ProgramEditorView()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    bankList1 = nullptr;
    programList = nullptr;
    banklist2 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
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



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
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



} /* KSP1 */

//[EndFile] You can add extra defines here...
//[/EndFile]

