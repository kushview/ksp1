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
#include "editor/Screens.h"
#include "InstrumentLoader.h"
#include "Locations.h"
//[/Headers]

#include "SamplerDisplay.h"


namespace KSP1 {
namespace Gui {

//[MiscUserDefs]

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

SamplerDisplay::SamplerDisplay ()
{
    addAndMakeVisible (comboBox = new ComboBox ("new combo box"));
    comboBox->setEditableText (false);
    comboBox->setJustificationType (Justification::centredLeft);
    comboBox->setTextWhenNothingSelected (TRANS("Chan"));
    comboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBox->addItem (TRANS("1"), 1);
    comboBox->addItem (TRANS("2"), 2);
    comboBox->addItem (TRANS("3"), 3);
    comboBox->addItem (TRANS("4"), 4);
    comboBox->addItem (TRANS("5"), 5);
    comboBox->addItem (TRANS("6"), 6);
    comboBox->addItem (TRANS("7"), 7);
    comboBox->addItem (TRANS("8"), 8);
    comboBox->addItem (TRANS("9"), 9);
    comboBox->addItem (TRANS("10"), 10);
    comboBox->addItem (TRANS("11"), 11);
    comboBox->addItem (TRANS("12"), 12);
    comboBox->addItem (TRANS("13"), 13);
    comboBox->addItem (TRANS("14"), 14);
    comboBox->addItem (TRANS("15"), 15);
    comboBox->addItem (TRANS("16"), 16);
    comboBox->addListener (this);

    addAndMakeVisible (title = new Label ("title",
                                          TRANS("New Instrument")));
    title->setTooltip (TRANS("Current Instrument"));
    title->setFont (Font (18.00f, Font::plain));
    title->setJustificationType (Justification::centredLeft);
    title->setEditable (false, true, false);
    title->setColour (Label::textColourId, Colours::chartreuse);
    title->setColour (TextEditor::textColourId, Colour (0xffe4e4e4));
    title->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    title->addListener (this);

    addAndMakeVisible (noteLabel = new Label ("note-label",
                                              TRANS("C0-1")));
    noteLabel->setTooltip (TRANS("Current Note"));
    noteLabel->setFont (Font (17.00f, Font::bold));
    noteLabel->setJustificationType (Justification::centredRight);
    noteLabel->setEditable (false, false, false);
    noteLabel->setColour (Label::textColourId, Colour (0xffb0c19f));
    noteLabel->setColour (TextEditor::textColourId, Colour (0xffe8e8e8));
    noteLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    noteLabel->setColour (TextEditor::highlightColourId, Colour (0x40545454));

    addAndMakeVisible (screen = new Component());
    screen->setName ("screen");

    addAndMakeVisible (progressBar = new DisplayProgressBar (*this));
    progressBar->setName ("progress-bar");


    //[UserPreSize]
    models = new Models();
    models->setInstrument (0, new Instrument ("New Instrument"));
    //[/UserPreSize]

    setSize (282, 147);


    //[Constructor]
    progressBar->setVisible (false);
    title->addMouseListener (this, false);
    screen = (Component*) Screen::create (*this, Screen::patternScreen);
    addAndMakeVisible (screen);
    currentLayer = 0;
    setNote (24);
    startTimer (1500);
    //dispatch = new SamplerDisplay::Dispatch (*this);
    //[/Constructor]
}

SamplerDisplay::~SamplerDisplay()
{
    //[Destructor_pre]
    stopTimer();
    //[/Destructor_pre]

    comboBox = nullptr;
    title = nullptr;
    noteLabel = nullptr;
    screen = nullptr;
    progressBar = nullptr;


    //[Destructor]
    dispatch = nullptr;
    models = nullptr;
    //[/Destructor]
}

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

    comboBox->setBounds (6, 4, 48, 16);
    title->setBounds (57, -1, 223, 24);
    noteLabel->setBounds (getWidth() - 6 - 59, 0, 59, 24);
    screen->setBounds (0, 24, getWidth() - 0, getHeight() - 24);
    progressBar->setBounds ((getWidth() / 2) + -183, (getHeight() / 2) + -22, 368, 41);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SamplerDisplay::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBox)
    {
        //[UserComboBoxCode_comboBox] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void SamplerDisplay::labelTextChanged (Label* labelThatHasChanged)
{
    //[UserlabelTextChanged_Pre]
    //[/UserlabelTextChanged_Pre]

    if (labelThatHasChanged == title)
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
    if (e.mods.isPopupMenu())
    {
    }
    //[/UserCode_mouseDown]
}

void SamplerDisplay::mouseDoubleClick (const MouseEvent& e)
{
    //[UserCode_mouseDoubleClick] -- Add your code here...
    if (e.originalComponent == title) {

    }
    //[/UserCode_mouseDoubleClick]
}



//[MiscUserCode]

InstrumentPtr SamplerDisplay::getInstrument() const
{
    return models->instrument();
}


ProgressSink& SamplerDisplay::progressBarSink()
{
    assert (progressBar.get());
    return *progressBar;
}

void
SamplerDisplay::runDispatchLoop()
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

void
SamplerDisplay::selectNote (int32 note, bool notify)
{
    KeyItem item = getInstrument()->getKey (note);
    if (Screen* s = dynamic_cast<Screen*> (screen.get()))
        if (notify) s->keySelectedEvent (item);

    setNote (note);
}

void
SamplerDisplay::setInstrument (InstrumentPtr inst)
{
    if (inst == nullptr)
        return;

    models->setInstrument (0, inst);
}

void
SamplerDisplay::setNote (int32 n)
{
    String letter = Midi::noteToText (n);
    letter << String("-") << String (currentLayer + 1);
    noteLabel->setText (letter, dontSendNotification);
    noteLabel->repaint();

    currentNote = n;
}

void
SamplerDisplay::setScreen (Screen::ID s)
{
    removeChildComponent (screen);
    this->screen = (Component*) Screen::create (*this, s);
    addAndMakeVisible (screen);
    resized();
}


void
SamplerDisplay::setTitle (const String& t)
{
    title->setText (t, dontSendNotification);
}


void
SamplerDisplay::timerCallback()
{

}

//[/MiscUserCode]


#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
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
  <COMBOBOX name="new combo box" id="50052ef19912822d" memberName="comboBox"
            virtualName="" explicitFocusOrder="0" pos="6 4 48 16" editable="0"
            layout="33" items="1&#10;2&#10;3&#10;4&#10;5&#10;6&#10;7&#10;8&#10;9&#10;10&#10;11&#10;12&#10;13&#10;14&#10;15&#10;16"
            textWhenNonSelected="Chan" textWhenNoItems="(no choices)"/>
  <LABEL name="title" id="b70ebe624c02371d" memberName="title" virtualName=""
         explicitFocusOrder="0" pos="57 -1 223 24" tooltip="Current Instrument"
         textCol="ff7fff00" edTextCol="ffe4e4e4" edBkgCol="0" labelText="New Instrument"
         editableSingleClick="0" editableDoubleClick="1" focusDiscardsChanges="0"
         fontname="Default font" fontsize="18" bold="0" italic="0" justification="33"/>
  <LABEL name="note-label" id="b73160220da4fa51" memberName="noteLabel"
         virtualName="" explicitFocusOrder="0" pos="6Rr 0 59 24" tooltip="Current Note"
         textCol="ffb0c19f" edTextCol="ffe8e8e8" edBkgCol="0" hiliteCol="40545454"
         labelText="C0-1" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="17"
         bold="1" italic="0" justification="34"/>
  <GENERICCOMPONENT name="screen" id="13e5e579773c6bbf" memberName="screen" virtualName=""
                    explicitFocusOrder="0" pos="0 24 0M 24M" class="Component" params=""/>
  <GENERICCOMPONENT name="progress-bar" id="f0d97f740b33cc6e" memberName="progressBar"
                    virtualName="" explicitFocusOrder="0" pos="-183C -22C 368 41"
                    class="DisplayProgressBar" params="*this"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile]
//[/EndFile]

}} /* namespace KSP1::Gui */
