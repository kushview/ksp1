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
#include <boost/bind.hpp>

#include "Instrument.h"
#include "editor/SamplerDisplay.h"
//[/Headers]

#include "SamplerView.h"


namespace KSP1 {

//[MiscUserDefs] You can add your own user definitions and misc code here...

class SamplerView::Updater :  public Timer
{
public:
    Updater (SamplerView& v) : view (v) { }
    void timerCallback() { view.onDisplayUpdate(); }

private:
    SamplerView& view;
};

//[/MiscUserDefs]

//==============================================================================
SamplerView::SamplerView ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    setName ("samplerView");
    display.reset (new SamplerDisplay());
    addAndMakeVisible (display.get());
    volLabel.reset (new Label ("volume-label",
                               TRANS("Volume\n")));
    addAndMakeVisible (volLabel.get());
    volLabel->setFont (Font (11.60f, Font::plain).withTypefaceStyle ("Regular"));
    volLabel->setJustificationType (Justification::centredRight);
    volLabel->setEditable (false, false, false);
    volLabel->setColour (Label::textColourId, Colour (0xffdadada));
    volLabel->setColour (TextEditor::textColourId, Colours::black);
    volLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    volLabel->setColour (TextEditor::highlightColourId, Colour (0x261151ee));

    meter.reset (new LevelMeter (2, true));
    addAndMakeVisible (meter.get());
    meter->setName ("meter");

    volume.reset (new Slider ("volume"));
    addAndMakeVisible (volume.get());
    volume->setTooltip (TRANS("Master Volume"));
    volume->setRange (-70, 6, 0.001);
    volume->setSliderStyle (Slider::RotaryVerticalDrag);
    volume->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    volume->setColour (Slider::backgroundColourId, Colour (0x00000000));
    volume->setColour (Slider::trackColourId, Colour (0x7fffffff));
    volume->setColour (Slider::rotarySliderFillColourId, Colour (0xff5591ad));
    volume->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff2a2a2a));
    volume->addListener (this);

    keyboard.reset (new KeyboardWidget (keyboardState));
    addAndMakeVisible (keyboard.get());
    keyboard->setName ("keyboard");

    outputLabel.reset (new Label ("OutputLabel",
                                  TRANS("Output")));
    addAndMakeVisible (outputLabel.get());
    outputLabel->setFont (Font (11.60f, Font::plain).withTypefaceStyle ("Regular"));
    outputLabel->setJustificationType (Justification::centredRight);
    outputLabel->setEditable (false, false, false);
    outputLabel->setColour (Label::textColourId, Colour (0xffdadada));
    outputLabel->setColour (TextEditor::textColourId, Colours::black);
    outputLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    ksp1Label.reset (new Label ("KSP1Label",
                                TRANS("KSP-1\n")));
    addAndMakeVisible (ksp1Label.get());
    ksp1Label->setFont (Font (20.00f, Font::plain).withTypefaceStyle ("Bold").withExtraKerningFactor (0.419f));
    ksp1Label->setJustificationType (Justification::centredLeft);
    ksp1Label->setEditable (false, false, false);
    ksp1Label->setColour (TextEditor::textColourId, Colours::black);
    ksp1Label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    ksp1Label->setBounds (3, 0, 196, 29);

    editButton.reset (new TextButton ("EditButton"));
    addAndMakeVisible (editButton.get());
    editButton->setButtonText (TRANS("E"));
    editButton->addListener (this);
    editButton->setColour (TextButton::buttonOnColourId, Colour (0xff1867ae));


    //[UserPreSize]
    display->connectUpdateClient (*this);
    keyboard->signalMidi().connect (boost::bind (&SamplerView::onKeyboardMidi, this, ::_1));
    keyboard->signalKeySelected().connect (boost::bind (&SamplerView::onKeySelected, this, ::_1));

    //[/UserPreSize]

    setSize (720, 420);


    //[Constructor] You can add your own custom stuff here..
    display->setScreen (Screen::editScreen);
    volume->setRange (-70.0f, 12.0f);
    updater = new Updater (*this);
    updater->startTimer (66);

    //[/Constructor]
}

SamplerView::~SamplerView()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    updater->stopTimer();
    updater = nullptr;
    //[/Destructor_pre]

    display = nullptr;
    volLabel = nullptr;
    meter = nullptr;
    volume = nullptr;
    keyboard = nullptr;
    outputLabel = nullptr;
    ksp1Label = nullptr;
    editButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SamplerView::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff8c3131));

    {
        int x = 0, y = 0, width = proportionOfWidth (1.0000f), height = proportionOfHeight (1.0000f);
        Colour fillColour1 = Colour (0xff747474), fillColour2 = Colour (0xff353535);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setGradientFill (ColourGradient (fillColour1,
                                       285.0f - 0.0f + x,
                                       2.0f - 0.0f + y,
                                       fillColour2,
                                       283.0f - 0.0f + x,
                                       651.0f - 0.0f + y,
                                       false));
        g.fillRect (x, y, width, height);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SamplerView::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    display->setBounds (4, 102, getWidth() - 8, getHeight() - 106);
    volLabel->setBounds (getWidth() - 319, 2, 52, 28);
    meter->setBounds (getWidth() - 168, 7, 136, 17);
    volume->setBounds (getWidth() - 264, 2, 28, 28);
    keyboard->setBounds (2, 97 - 68, getWidth() - 8, 68);
    outputLabel->setBounds (getWidth() - 222, 1, 48, 28);
    editButton->setBounds (getWidth() - 6 - 18, 7, 18, 18);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SamplerView::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == volume.get())
    {
        //[UserSliderCode_volume] -- add your slider handling code here..
        //[/UserSliderCode_volume]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void SamplerView::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == editButton.get())
    {
        //[UserButtonCode_editButton] -- add your button handler code here..
        editButton->setToggleState (! editButton->getToggleState(), dontSendNotification);
        //[/UserButtonCode_editButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void SamplerView::setVolume (float db, bool notify)
{
    NotificationType n = notify ? sendNotification : dontSendNotification;
    volume->setValue (db, n);
}

#if 0
void
SamplerView::acceptAtom (lvtk::Atom atom)
{
    if (atom.type() == portWriter.atom_Blank ||
        atom.type() == portWriter.atom_Resource)
    {
        lvtk::AtomObject obj (atom.as_object());
        if (obj.otype() == portWriter.sampler_Key) {
           // LayerBrowser->clearLayers();
            std::clog << "got key\n";
        }
        else if (obj.otype() == portWriter.sampler_Layer)
        {
            std::clog << "got layer\n";
        }
    }
}
#endif

void SamplerView::onKeyboardMidi (const MidiMessage& midi)
{
    if (midi.isNoteOn (false))
    {
        display->selectNote (midi.getNoteNumber());
        updateControls (dontSendNotification);
    }
}

void SamplerView::onKeySelected (int k)
{
    display->setNote (k);
    stabilizeView();
}

void SamplerView::onDisplayUpdate()
{

}

void SamplerView::loadFile (const File& file)
{

}

void SamplerView::layerChosen()
{
#if 0
    if (EditScreen* screen = dynamic_cast<EditScreen*> (display->findChildWithID ("edit-screen")))
    {
        LayerItem layer (layersListBox->getSelectedLayer());
        screen->resized();
    }
#endif
    updateControls();
}

void SamplerView::updateControls (NotificationType n)
{
    InstrumentPtr instrument (display->getInstrument());
    volume->getValueObject().referTo (instrument->getPropertyAsValue (Tags::volume));
}

InstrumentPtr SamplerView::getInstrument (const int) const
{
    return display->getInstrument();
}

void SamplerView::valueChanged (Value& value)
{
    if (value.refersToSameSourceAs (activeSound))
        stabilizeView();
}

void SamplerView::setInstrument (InstrumentPtr i)
{
    activeSound.removeListener (this);
    display->setInstrument (i);
    if (auto instrument = getInstrument())
    {
        activeSound.referTo (instrument->getPropertyAsValue ("activeSound"));
        activeSound.addListener (this);
    }

    stabilizeView();
}

void SamplerView::stabilizeView()
{
    updateControls (dontSendNotification);
}

void SamplerView::setMainRMS (const float rmsL, const float rmsR)
{
    meter->setValue (0, rmsL);
    meter->setValue (1, rmsR);
    meter->refresh();
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SamplerView" template="../../jucer/Templates/GuiTemplate.cpp"
                 componentName="samplerView" parentClasses="public Component, public DragAndDropContainer, private Value::Listener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="0"
                 snapShown="1" overlayOpacity="0.330" fixedSize="1" initialWidth="720"
                 initialHeight="420">
  <BACKGROUND backgroundColour="ff8c3131">
    <RECT pos="0 0 100% 100%" fill="linear: 285 2, 283 651, 0=ff747474, 1=ff353535"
          hasStroke="0"/>
  </BACKGROUND>
  <JUCERCOMP name="display" id="56c24b8a829e534f" memberName="display" virtualName=""
             explicitFocusOrder="0" pos="4 102 8M 106M" sourceFile="SamplerDisplay.cpp"
             constructorParams=""/>
  <LABEL name="volume-label" id="73228f61d5b45b64" memberName="volLabel"
         virtualName="" explicitFocusOrder="0" pos="319R 2 52 28" textCol="ffdadada"
         edTextCol="ff000000" edBkgCol="0" hiliteCol="261151ee" labelText="Volume&#10;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.16e1" kerning="0" bold="0"
         italic="0" justification="34"/>
  <GENERICCOMPONENT name="meter" id="bfda63f9ccd27d97" memberName="meter" virtualName=""
                    explicitFocusOrder="0" pos="168R 7 136 17" class="LevelMeter"
                    params="2, true"/>
  <SLIDER name="volume" id="47a6f6acb9d0b4b3" memberName="volume" virtualName=""
          explicitFocusOrder="0" pos="264R 2 28 28" tooltip="Master Volume"
          bkgcol="0" trackcol="7fffffff" rotarysliderfill="ff5591ad" rotaryslideroutline="ff2a2a2a"
          min="-7e1" max="6" int="1e-3" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <GENERICCOMPONENT name="keyboard" id="99aac150ed63807" memberName="keyboard" virtualName=""
                    explicitFocusOrder="0" pos="2 97r 8M 68" class="KeyboardWidget"
                    params="keyboardState"/>
  <LABEL name="OutputLabel" id="277ce632dc5f2ace" memberName="outputLabel"
         virtualName="" explicitFocusOrder="0" pos="222R 1 48 28" textCol="ffdadada"
         edTextCol="ff000000" edBkgCol="0" labelText="Output" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="1.16e1" kerning="0" bold="0" italic="0" justification="34"/>
  <LABEL name="KSP1Label" id="ef55d3420a00b74f" memberName="ksp1Label"
         virtualName="" explicitFocusOrder="0" pos="3 0 196 29" edTextCol="ff000000"
         edBkgCol="0" labelText="KSP-1&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="2e1"
         kerning="4.19e-1" bold="1" italic="0" justification="33" typefaceStyle="Bold"/>
  <TEXTBUTTON name="EditButton" id="50b83eadd71f2375" memberName="editButton"
              virtualName="" explicitFocusOrder="0" pos="6Rr 7 18 18" bgColOn="ff1867ae"
              buttonText="E" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif



} /* KSP1 */

//[EndFile] You can add extra defines here...
//[/EndFile]

