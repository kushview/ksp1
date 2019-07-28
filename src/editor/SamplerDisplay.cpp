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
#include "editor/Screens.h"
#include "editor/ScreenManager.h"
#include "InstrumentLoader.h"
//[/Headers]

#include "SamplerDisplay.h"


namespace KSP1 {

//[MiscUserDefs] You can add your own user definitions and misc code here...

class SamplerDisplay::Models
{
public:

    Models() { }

    ~Models()
    {

    }

    InstrumentPtr instrument (int index = 0) {
        return this->instrments [index];
    }

    void setInstrument (int index, InstrumentPtr i) {
        instrments.set (index, i);
    }

private:
    ReferenceCountedArray<Instrument> instrments;
};

class SamplerDisplay::Dispatch : public Timer
{
public:

    Dispatch (SamplerDisplay& d)
        : display (d)
    {
        setFrequencyMillis (64);
        startTimer (milliseconds);
    }

    ~Dispatch()
    {
        stopTimer();
    }

    void setFrequencyMillis (int millis)
    {
        const bool wasRunning = isTimerRunning();

        if (wasRunning)
            stopTimer();

        milliseconds = millis;

        if (wasRunning && milliseconds > 0)
            startTimer (milliseconds);
    }

    void timerCallback () {
        display.runDispatchLoop();
    }

    /** refresh rate in hertz */
    double refreshRate() const
    {
        if (milliseconds <= 0)
            return 0.0f;

        return  1000.f / (double) milliseconds;
    }

private:

    SamplerDisplay& display;
    int milliseconds;

};

class DisplayProgressBar :  public ProgressBar,
                            public ProgressSink
{
    SamplerDisplay& display;

public:

    DisplayProgressBar (SamplerDisplay& parent)
        : ProgressBar (val),
          display (parent),
          val(0.0f)
    {
        setPercentageDisplay (true);
        setInterceptsMouseClicks (false, false);
        setAlwaysOnTop (true);
        setVisible (false);
        setAlpha (0.6);
    }

    void handleProgress (float p)
    {
        val = p;
        repaint();
        MessageManager::getInstance()->runDispatchLoopUntil (1);
    }

    void handleStatus (const String& msg)
    {
        display.setTitle (msg);
    }

    void handleProgressStart()
    {
        val = 0.0f;
        setVisible (true);
    }

    void handleProgressFinished()
    {
        setVisible (false);
        display.setTitle ("Finished Loading...");
    }

private:
    double val;
};

//[/MiscUserDefs]

//==============================================================================
SamplerDisplay::SamplerDisplay ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    title.reset (new Label ("title",
                            TRANS("Default Instrument")));
    addAndMakeVisible (title.get());
    title->setTooltip (TRANS("Current Instrument"));
    title->setFont (Font (18.00f, Font::plain).withTypefaceStyle ("Regular"));
    title->setJustificationType (Justification::centredLeft);
    title->setEditable (false, true, false);
    title->setColour (Label::textColourId, Colours::chartreuse);
    title->setColour (TextEditor::textColourId, Colour (0xffe4e4e4));
    title->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    title->addListener (this);

    title->setBounds (1, -1, 279, 24);

    noteLabel.reset (new Label ("note-label",
                                TRANS("C0-1")));
    addAndMakeVisible (noteLabel.get());
    noteLabel->setTooltip (TRANS("Current Note"));
    noteLabel->setFont (Font (17.00f, Font::plain).withTypefaceStyle ("Bold"));
    noteLabel->setJustificationType (Justification::centredRight);
    noteLabel->setEditable (false, false, false);
    noteLabel->setColour (Label::textColourId, Colour (0xffb0c19f));
    noteLabel->setColour (TextEditor::textColourId, Colour (0xffe8e8e8));
    noteLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    noteLabel->setColour (TextEditor::highlightColourId, Colour (0x40545454));

    screen.reset (new Component());
    addAndMakeVisible (screen.get());
    screen->setName ("screen");

    progressBar.reset (new DisplayProgressBar (*this));
    addAndMakeVisible (progressBar.get());
    progressBar->setName ("progress-bar");


    //[UserPreSize]
    screens = new ScreenManager();
    currentScreen = nullptr;
    models = new Models();
    models->setInstrument (0, new Instrument ("New Instrument"));
    //[/UserPreSize]

    setSize (282, 147);


    //[Constructor] You can add your own custom stuff here..
    progressBar->setVisible (false);
    title->addMouseListener (this, false);
    currentLayer = 0;
    setNote (24);
    startTimer (1500);
    setScreen (Screen::editScreen);
    //[/Constructor]
}

SamplerDisplay::~SamplerDisplay()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    stopTimer();
    //[/Destructor_pre]

    title = nullptr;
    noteLabel = nullptr;
    screen = nullptr;
    progressBar = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    dispatch = nullptr;
    models = nullptr;
    //[/Destructor]
}

//==============================================================================
void SamplerDisplay::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xa00e0e0e));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SamplerDisplay::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    noteLabel->setBounds (getWidth() - 6 - 59, 0, 59, 24);
    screen->setBounds (0, 24, getWidth() - 0, getHeight() - 24);
    progressBar->setBounds ((getWidth() / 2) + -183, (getHeight() / 2) + -22, 368, 41);
    //[UserResized] Add your own custom resize handling here..
    if (currentScreen)
        currentScreen->setBounds (screen->getBoundsInParent());
    //[/UserResized]
}

void SamplerDisplay::labelTextChanged (Label* labelThatHasChanged)
{
    //[UserlabelTextChanged_Pre]
    //[/UserlabelTextChanged_Pre]

    if (labelThatHasChanged == title.get())
    {
        //[UserLabelCode_title] -- add your label text handling code here..
        //[/UserLabelCode_title]
    }

    //[UserlabelTextChanged_Post]
    //[/UserlabelTextChanged_Post]
}

void SamplerDisplay::mouseDown (const MouseEvent& e)
{
    //[UserCode_mouseDown] -- Add your code here...
    //[/UserCode_mouseDown]
}

void SamplerDisplay::mouseDoubleClick (const MouseEvent& e)
{
    //[UserCode_mouseDoubleClick] -- Add your code here...
    //[/UserCode_mouseDoubleClick]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

InstrumentPtr SamplerDisplay::getInstrument() const
{
    return models->instrument();
}

ProgressSink& SamplerDisplay::progressBarSink()
{
    jassert (progressBar.get());
    return *progressBar;
}

void SamplerDisplay::runDispatchLoop()
{
#if 1
    if (nullptr != getInstrument() && currentNote != getInstrument()->currentKeyId())
    {
        currentNote = getInstrument()->currentKeyId();
        selectNote (currentNote, true);
    }
#endif
    updateSignal(); // emit signal to clients
}

KeyItem SamplerDisplay::selectedKey()
{
    KeyItem key (getInstrument()->getKey (selectedNote()));
    return key;
}

LayerItem SamplerDisplay::selectedLayer()
{
    KeyItem item (selectedKey());
    int whichLayer = currentLayer > item.countLayers()
                   ? item.countLayers() : (int) currentLayer;
    return getInstrument()->getLayer (item.getNote(), whichLayer);
}

void SamplerDisplay::selectNote (int32 note, bool notify)
{
   #if 0
    KeyItem item = getInstrument()->getKey (note);
    if (currentScreen && notify)
        currentScreen->keySelectedEvent (item);
   #endif

    setNote (note);
}

void SamplerDisplay::setInstrument (InstrumentPtr inst)
{
    if (inst == nullptr)
        return;

    models->setInstrument (0, inst);
}

void SamplerDisplay::setNote (int32 n)
{
    String letter = MidiMessage::getMidiNoteName (n, false, true, 4);
    noteLabel->setText (letter, dontSendNotification);
    noteLabel->repaint();
    currentNote = n;
}

void SamplerDisplay::setScreen (Screen::ID s)
{
    if (currentScreen)
        removeChildComponent (currentScreen);
    this->currentScreen = screens->getScreen (*this, (int) s);
    if (currentScreen)
    {
        addAndMakeVisible (currentScreen);
    }
    resized();
}


void SamplerDisplay::setTitle (const String& t)
{
    title->setText (t, dontSendNotification);
}


void SamplerDisplay::timerCallback()
{

}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SamplerDisplay" template="../../jucer/Templates/GuiTemplate.cpp"
                 componentName="" parentClasses="public Component, public Timer"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="282"
                 initialHeight="147">
  <METHODS>
    <METHOD name="mouseDoubleClick (const MouseEvent&amp; e)"/>
    <METHOD name="mouseDown (const MouseEvent&amp; e)"/>
  </METHODS>
  <BACKGROUND backgroundColour="a00e0e0e"/>
  <LABEL name="title" id="b70ebe624c02371d" memberName="title" virtualName=""
         explicitFocusOrder="0" pos="1 -1 279 24" tooltip="Current Instrument"
         textCol="ff7fff00" edTextCol="ffe4e4e4" edBkgCol="0" labelText="Default Instrument"
         editableSingleClick="0" editableDoubleClick="1" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.8e1" kerning="0" bold="0"
         italic="0" justification="33"/>
  <LABEL name="note-label" id="b73160220da4fa51" memberName="noteLabel"
         virtualName="" explicitFocusOrder="0" pos="6Rr 0 59 24" tooltip="Current Note"
         textCol="ffb0c19f" edTextCol="ffe8e8e8" edBkgCol="0" hiliteCol="40545454"
         labelText="C0-1" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="1.7e1"
         kerning="0" bold="1" italic="0" justification="34" typefaceStyle="Bold"/>
  <GENERICCOMPONENT name="screen" id="13e5e579773c6bbf" memberName="screen" virtualName=""
                    explicitFocusOrder="0" pos="0 24 0M 24M" class="Component" params=""/>
  <GENERICCOMPONENT name="progress-bar" id="f0d97f740b33cc6e" memberName="progressBar"
                    virtualName="" explicitFocusOrder="0" pos="-183C -22C 368 41"
                    class="DisplayProgressBar" params="*this"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif



} /* KSP1 */

//[EndFile] You can add extra defines here...
//[/EndFile]

