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

#include "../Instrument.h"
#include "ProgramsListBox.h"
#include "SamplerDisplay.h"
#include "Screens.h"
//[/Headers]

#include "QuickBrowser.h"

namespace KSP1 {

//[MiscUserDefs] You can add your own user definitions and misc code here...
#if 0
    // FIXME:
    QuickBrowser::QuickBrowser (Programming &progs, Instrument &i)
        : programs (progs), inst (&i)
    {
        addAndMakeVisible (listBox = new ProgramsListBox (programs));
        listBox->setName ("list-box");

        addAndMakeVisible (textButton = new TextButton ("new button"));
        textButton->setButtonText ("X");
        textButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
        textButton->addListener (this);


        //[UserPreSize]
        listBox->signalEngaged().connect (boost::bind (&QuickBrowser::loadItem, this, ::_1));
        //[/UserPreSize]

        setSize (600, 400);
    }

    QuickBrowser::QuickBrowser (Screen& screen)
        : programs (screen.display().globals().programs()),
          inst (screen.display().instrument())
    {
        addAndMakeVisible (listBox = new ProgramsListBox (programs));
        listBox->setName ("list-box");

        addAndMakeVisible (textButton = new TextButton ("new button"));
        textButton->setButtonText ("X");
        textButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
        textButton->addListener (this);


        //[UserPreSize]
        listBox->signalEngaged().connect (boost::bind (&QuickBrowser::loadItem, this, ::_1));
        //[/UserPreSize]

        setSize (600, 400);
    }

    void
    QuickBrowser::loadItem (const Programming::Item& prog)
    {
        SamplerDisplay* display = findParentComponentOfClass<SamplerDisplay>();
        if (display != nullptr && inst != nullptr) {
            this->setVisible (false);
            repaint();
            inst->load (prog.asFile(), display->progressBarSink());
            delete this;
        }
    }

//[/MiscUserDefs]

//==============================================================================
QuickBrowser::QuickBrowser (Programming& progs)
    : programs (progs)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    listBox.reset (new ProgramsListBox (programs));
    addAndMakeVisible (listBox.get());
    listBox->setName ("list-box");

    textButton.reset (new TextButton ("new button"));
    addAndMakeVisible (textButton.get());
    textButton->setButtonText (TRANS("X"));
    textButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    textButton->addListener (this);


    //[UserPreSize]
        listBox->signalEngaged().connect (boost::bind (&QuickBrowser::loadItem, this, ::_1));
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    inst = nullptr;
    //[/Constructor]
}

QuickBrowser::~QuickBrowser()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    listBox = nullptr;
    textButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void QuickBrowser::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xc92b2b2b));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void QuickBrowser::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    listBox->setBounds (8, 8, getWidth() - 15, getHeight() - 20);
    textButton->setBounds (getWidth() - 31, 8, 24, 16);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void QuickBrowser::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == textButton.get())
    {
        //[UserButtonCode_textButton] -- add your button handler code here..
        //[/UserButtonCode_textButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#    if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="QuickBrowser" template="../../jucer/Templates/GuiTemplate.cpp"
                 componentName="" parentClasses="public Component" constructorParams="Programming&amp; progs"
                 variableInitialisers="programs (progs)" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="600"
                 initialHeight="400">
  <BACKGROUND backgroundColour="c92b2b2b"/>
  <GENERICCOMPONENT name="list-box" id="c1364a776e8fd643" memberName="listBox" virtualName=""
                    explicitFocusOrder="0" pos="8 8 15M 20M" class="ProgramsListBox"
                    params="programs"/>
  <TEXTBUTTON name="new button" id="57755ddb8576cbe8" memberName="textButton"
              virtualName="" explicitFocusOrder="0" pos="31R 8 24 16" buttonText="X"
              connectedEdges="15" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#    endif



} /* KSP1 */

//[EndFile] You can add extra defines here...
#endif
//[/EndFile]
