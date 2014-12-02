/*
    Copyright 2014 Kushview, LLC.  All rights reserved
    This is an automatically generated GUI class created by the Introjucer!
*/

//[Headers]
#include <boost/bind.hpp>
#include "editor/Workspace.h"
//[/Headers]

#include "SamplerPluginView.h"


namespace KSP1 {
namespace Gui {

//[MiscUserDefs]

//[/MiscUserDefs]

SamplerPluginView::SamplerPluginView (SamplerEditor& ed)
    : world (ed.globals()),
      owner (ed)
{
    addAndMakeVisible (workspace = new Workspace (owner));
    workspace->setName ("new component");

    addAndMakeVisible (drumbotLabel = new Label ("drumbot-label",
                                                 TRANS("KSP-1")));
    drumbotLabel->setFont (Font (Font::getDefaultMonospacedFontName(), 26.00f, Font::bold));
    drumbotLabel->setJustificationType (Justification::centred);
    drumbotLabel->setEditable (false, false, false);
    drumbotLabel->setColour (Label::textColourId, Colour (0x9ee4ffcd));
    drumbotLabel->setColour (TextEditor::textColourId, Colours::black);
    drumbotLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    drumbotLabel->setColour (TextEditor::highlightColourId, Colour (0x4011ee5a));

    addAndMakeVisible (textButton = new TextButton ("new button"));
    textButton->setButtonText (TRANS("EDIT"));
    textButton->addListener (this);

    addAndMakeVisible (textButton2 = new TextButton ("new button"));
    textButton2->setButtonText (TRANS("MIX"));
    textButton2->addListener (this);

    addAndMakeVisible (textButton3 = new TextButton ("new button"));
    textButton3->setButtonText (TRANS("PROP"));
    textButton3->addListener (this);

    addAndMakeVisible (textButton4 = new TextButton ("new button"));
    textButton4->setButtonText (TRANS("S"));
    textButton4->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (964, 572);


    //[Constructor]
    workspace->switchPanel (Panel::samplerPanel);
    constrain.setMinimumSize (790, 400);
    //[/Constructor]
}

SamplerPluginView::~SamplerPluginView()
{
    //[Destructor_pre]
    setLookAndFeel (nullptr);
    //[/Destructor_pre]

    workspace = nullptr;
    drumbotLabel = nullptr;
    textButton = nullptr;
    textButton2 = nullptr;
    textButton3 = nullptr;
    textButton4 = nullptr;


    //[Destructor]
    //[/Destructor]
}

void SamplerPluginView::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..

    //[/UserPaint]
}

void SamplerPluginView::resized()
{
    workspace->setBounds (2, 28, getWidth() - 4, getHeight() - 30);
    drumbotLabel->setBounds (7, 1, 164, 24);
    textButton->setBounds (192, 4, 40, 20);
    textButton2->setBounds (237, 4, 40, 20);
    textButton3->setBounds (283, 4, 40, 20);
    textButton4->setBounds (getWidth() - 4 - 20, 4, 20, 20);
    //[UserResized] Add your own custom resize handling here..
    constrain.checkComponentBounds(this);
    //[/UserResized]
}

void SamplerPluginView::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == textButton)
    {
        //[UserButtonCode_textButton] -- add your button handler code here..
        //[/UserButtonCode_textButton]
    }
    else if (buttonThatWasClicked == textButton2)
    {
        //[UserButtonCode_textButton2] -- add your button handler code here..
        //[/UserButtonCode_textButton2]
    }
    else if (buttonThatWasClicked == textButton3)
    {
        //[UserButtonCode_textButton3] -- add your button handler code here..
        //[/UserButtonCode_textButton3]
    }
    else if (buttonThatWasClicked == textButton4)
    {
        //[UserButtonCode_textButton4] -- add your button handler code here..
        //[/UserButtonCode_textButton4]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode]
SamplerDisplay*
SamplerPluginView::display()
{
    if (workspace)
        return workspace->display();
    return nullptr;
}

//[/MiscUserCode]


#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SamplerPluginView" template="../../jucer/Templates/GuiTemplate.cpp"
                 componentName="" parentClasses="public Component" constructorParams="SamplerEditor&amp; ed"
                 variableInitialisers="world (ed.globals())&#10;owner (ed)" snapPixels="4"
                 snapActive="1" snapShown="1" overlayOpacity="0.330" fixedSize="0"
                 initialWidth="964" initialHeight="572">
  <BACKGROUND backgroundColour="858585"/>
  <GENERICCOMPONENT name="new component" id="f96beadb57804561" memberName="workspace"
                    virtualName="" explicitFocusOrder="0" pos="2 28 4M 30M" class="Workspace"
                    params="owner"/>
  <LABEL name="drumbot-label" id="8603053ab463477e" memberName="drumbotLabel"
         virtualName="" explicitFocusOrder="0" pos="7 1 164 24" textCol="9ee4ffcd"
         edTextCol="ff000000" edBkgCol="0" hiliteCol="4011ee5a" labelText="KSP-1"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default monospaced font" fontsize="26" bold="1" italic="0"
         justification="36"/>
  <TEXTBUTTON name="new button" id="37b59de3512cb3b3" memberName="textButton"
              virtualName="" explicitFocusOrder="0" pos="192 4 40 20" buttonText="EDIT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="78781190bb21b51e" memberName="textButton2"
              virtualName="" explicitFocusOrder="0" pos="237 4 40 20" buttonText="MIX"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="ede5522ec0008f50" memberName="textButton3"
              virtualName="" explicitFocusOrder="0" pos="283 4 40 20" buttonText="PROP"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="33b38824713c8cb4" memberName="textButton4"
              virtualName="" explicitFocusOrder="0" pos="4Rr 4 20 20" buttonText="S"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile]

//[/EndFile]

}} /* namespace KSP1::Gui */
