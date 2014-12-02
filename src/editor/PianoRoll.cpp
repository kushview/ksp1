/*
    Copyright 2014 Kushview, LLC.  All rights reserved
    This is an automatically generated GUI class created by the Introjucer!
*/

//[Headers]
#include "editor/SamplerDisplay.h"
#include "editor/ProgramsListBox.h"
#include "editor/Screens.h"
#include "editor/QuickBrowser.h"
#include "Instrument.h"
//[/Headers]

#include "PianoRoll.h"


namespace KSP1 {
namespace Gui {

//[MiscUserDefs]

//[/MiscUserDefs]

PianoRollScreen::PianoRollScreen (SamplerDisplay& owner)
    : Screen (owner, "Test Screen", Screen::editScreen)
{
    addAndMakeVisible (buttonAddSample = new TextButton ("add-sample-button"));
    buttonAddSample->setTooltip (TRANS("Add a Sample"));
    buttonAddSample->setButtonText (TRANS("+"));
    buttonAddSample->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    buttonAddSample->addListener (this);
    buttonAddSample->setColour (TextButton::buttonColourId, Colour (0xc7282828));
    buttonAddSample->setColour (TextButton::buttonOnColourId, Colour (0xe5cbcbcb));
    buttonAddSample->setColour (TextButton::textColourOnId, Colour (0xff858585));
    buttonAddSample->setColour (TextButton::textColourOffId, Colour (0xffe2e2e2));

    addAndMakeVisible (buttonRemoveLayer = new TextButton ("button-remove-layer"));
    buttonRemoveLayer->setTooltip (TRANS("Remove Sample"));
    buttonRemoveLayer->setButtonText (TRANS("-"));
    buttonRemoveLayer->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    buttonRemoveLayer->addListener (this);
    buttonRemoveLayer->setColour (TextButton::buttonColourId, Colour (0xc72d2d2d));
    buttonRemoveLayer->setColour (TextButton::buttonOnColourId, Colour (0xe5ededed));
    buttonRemoveLayer->setColour (TextButton::textColourOnId, Colour (0xff858585));
    buttonRemoveLayer->setColour (TextButton::textColourOffId, Colour (0xffe2e2e2));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor]
    lastNote = display().selectedKey().getNote();
    //[/Constructor]
}

PianoRollScreen::~PianoRollScreen()
{
    //[Destructor_pre]
    //[/Destructor_pre]

    buttonAddSample = nullptr;
    buttonRemoveLayer = nullptr;


    //[Destructor]
    //[/Destructor]
}

void PianoRollScreen::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::black);

    //[UserPaint] Add your own custom painting code here..

    //[/UserPaint]
}

void PianoRollScreen::resized()
{
    buttonAddSample->setBounds (getWidth() - 56, getHeight() - 18, 24, 16);
    buttonRemoveLayer->setBounds (getWidth() - 28, getHeight() - 18, 24, 16);
    //[UserResized] Add your own custom resize handling here..

    //[/UserResized]
}

void PianoRollScreen::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    KeyItem key (display().selectedKey());
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == buttonAddSample)
    {
        //[UserButtonCode_buttonAddSample] -- add your button handler code here..
#if 0
        QuickBrowser* box = new QuickBrowser (*this);
        addAndMakeVisible (box);
        box->setBounds (getLocalBounds().reduced (4));
#else
        FileChooser chooser ("Open media", File(String ("/Users/Shared/SimBeatThang/EDMContent/samples")), "*.btdk;*.xml;*.wav;*.aiff", false);
        if (chooser.browseForMultipleFilesToOpen())
        {
            for (const File& f : chooser.getResults())
            {
                if (f.getFileExtension() == ".xml" ||
                    f.getFileExtension() == ".btdk")
                {
                    ProgressSink& sink = display().progressBarSink();
                    display().getInstrument()->load (f, sink);
                }
                else
                    key.addLayer (f);
            }
        }
#endif
        //[/UserButtonCode_buttonAddSample]
    }
    else if (buttonThatWasClicked == buttonRemoveLayer)
    {
        //[UserButtonCode_buttonRemoveLayer] -- add your button handler code here..
        //key.removeLayer (display().currentLayer);
        //[/UserButtonCode_buttonRemoveLayer]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode]
void
PianoRollScreen::keySelectedEvent (const KeyItem& item)
{

}

void PianoRollScreen::timerCallback()
{
}

void PianoRollScreen::updateComponents()
{
}

void
PianoRollScreen::onDisplayUpdate()
{

}

//[/MiscUserCode]


#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PianoRollScreen" template="../../jucer/Templates/GuiTemplate.cpp"
                 componentName="" parentClasses="public Screen, public Timer"
                 constructorParams="SamplerDisplay&amp; owner" variableInitialisers="Screen (owner, &quot;Test Screen&quot;, Screen::editScreen)"
                 snapPixels="2" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff000000"/>
  <TEXTBUTTON name="add-sample-button" id="2a6414b50430161c" memberName="buttonAddSample"
              virtualName="" explicitFocusOrder="0" pos="56R 18R 24 16" tooltip="Add a Sample"
              bgColOff="c7282828" bgColOn="e5cbcbcb" textCol="ff858585" textColOn="ffe2e2e2"
              buttonText="+" connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="button-remove-layer" id="9a41684d52c0c346" memberName="buttonRemoveLayer"
              virtualName="" explicitFocusOrder="0" pos="28R 18R 24 16" tooltip="Remove Sample"
              bgColOff="c72d2d2d" bgColOn="e5ededed" textCol="ff858585" textColOn="ffe2e2e2"
              buttonText="-" connectedEdges="15" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile]
//[/EndFile]

}} /* namespace KSP1::Gui */
