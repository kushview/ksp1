/*
    Copyright 2014, Kushview, LLC
    This is an automatically generated GUI class created by the Introjucer!
*/

//[Headers]


#include "ProgramsListBox.h"
#include "../Instrument.h"
#include "SamplerDisplay.h"
#include "Screens.h"
//[/Headers]

#include "QuickBrowser.h"


namespace KSP1 {
namespace Gui {


//[MiscUserDefs]
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

QuickBrowser::QuickBrowser (Programming& progs)
    : programs (progs)
{
    addAndMakeVisible (listBox = new ProgramsListBox (programs));
    listBox->setName ("list-box");

    addAndMakeVisible (textButton = new TextButton ("new button"));
    textButton->setButtonText (TRANS("X"));
    textButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    textButton->addListener (this);


    //[UserPreSize]
        listBox->signalEngaged().connect (boost::bind (&QuickBrowser::loadItem, this, ::_1));
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor]
    inst = nullptr;
    //[/Constructor]
}

QuickBrowser::~QuickBrowser()
{
    //[Destructor_pre]
    //[/Destructor_pre]

    listBox = nullptr;
    textButton = nullptr;


    //[Destructor]
    //[/Destructor]
}

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
    listBox->setBounds (8, 8, getWidth() - 15, getHeight() - 20);
    textButton->setBounds (getWidth() - 31, 8, 24, 16);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void QuickBrowser::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == textButton)
    {
        //[UserButtonCode_textButton] -- add your button handler code here..
        delete this;
        //[/UserButtonCode_textButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode]
//[/MiscUserCode]


#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
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
#endif


//[EndFile]
#endif
//[/EndFile]

}} /* namespace KSP1::Gui */
