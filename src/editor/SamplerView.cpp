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
#include "editor/Screens.h"
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
    volume->setColour (Slider::thumbColourId, Colours::white);
    volume->setColour (Slider::trackColourId, Colour (0x7fffffff));
    volume->setColour (Slider::rotarySliderFillColourId, Colours::white);
    volume->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff2a2a2a));
    volume->setColour (Slider::textBoxHighlightColourId, Colour (0x66df1010));
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
    ksp1Label->setFont (Font (20.00f, Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (0.419f));
    ksp1Label->setJustificationType (Justification::centredLeft);
    ksp1Label->setEditable (false, false, false);
    ksp1Label->setColour (TextEditor::textColourId, Colours::black);
    ksp1Label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    ksp1Label->setBounds (3, 0, 196, 36);

    soundsButton.reset (new TextButton ("SoundsButton"));
    addAndMakeVisible (soundsButton.get());
    soundsButton->setButtonText (TRANS("Sounds"));
    soundsButton->setConnectedEdges (Button::ConnectedOnRight);
    soundsButton->setRadioGroupId (1);
    soundsButton->addListener (this);
    soundsButton->setColour (TextButton::buttonOnColourId, Colour (0xff1867ae));

    layersButton.reset (new TextButton ("LayersButton"));
    addAndMakeVisible (layersButton.get());
    layersButton->setButtonText (TRANS("Layers"));
    layersButton->setConnectedEdges (Button::ConnectedOnLeft);
    layersButton->setRadioGroupId (1);
    layersButton->addListener (this);
    layersButton->setColour (TextButton::buttonOnColourId, Colour (0xff1867ae));

    editButton.reset (new TextButton ("EditButton"));
    addAndMakeVisible (editButton.get());
    editButton->setButtonText (TRANS("Edit"));
    editButton->setRadioGroupId (1);
    editButton->addListener (this);
    editButton->setColour (TextButton::buttonOnColourId, Colour (0xff1867ae));

    editButton->setBounds (4, 125, 36, 16);


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
    soundsButton = nullptr;
    layersButton = nullptr;
    editButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SamplerView::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff373737));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SamplerView::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    display->setBounds (4, 150, getWidth() - 8, getHeight() - 154);
    volLabel->setBounds (getWidth() - 293, 0, 52, 36);
    meter->setBounds (getWidth() - 142, 11, 136, 17);
    volume->setBounds (getWidth() - 238, 4, 28, 30);
    keyboard->setBounds (2, 116 - 73, getWidth() - 8, 73);
    outputLabel->setBounds (getWidth() - 196, 0, 48, 36);
    soundsButton->setBounds ((getWidth() / 2) - 36, 125, 36, 16);
    layersButton->setBounds ((getWidth() / 2) + 1, 125, 36, 16);
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

    if (buttonThatWasClicked == soundsButton.get())
    {
        //[UserButtonCode_soundsButton] -- add your button handler code here..
        soundsButton->setToggleState (! soundsButton->getToggleState(), dontSendNotification);
        if (soundsButton->getToggleState())
            display->setScreen (Screen::editScreen, 0);
        //[/UserButtonCode_soundsButton]
    }
    else if (buttonThatWasClicked == layersButton.get())
    {
        //[UserButtonCode_layersButton] -- add your button handler code here..
        layersButton->setToggleState (! layersButton->getToggleState(), dontSendNotification);
        if (layersButton->getToggleState())
            display->setScreen (Screen::editScreen, 1);
        //[/UserButtonCode_layersButton]
    }
    else if (buttonThatWasClicked == editButton.get())
    {
        //[UserButtonCode_editButton] -- add your button handler code here..
        //[/UserButtonCode_editButton]
    }

    //[UserbuttonClicked_Post]
    editButton->setToggleState (layersButton->getToggleState() || soundsButton->getToggleState(),
                                dontSendNotification);
    
    if (editButton->getToggleState())
    {
        
    }
    else
    {
        
    }
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
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="720"
                 initialHeight="420">
  <BACKGROUND backgroundColour="ff373737"/>
  <JUCERCOMP name="display" id="56c24b8a829e534f" memberName="display" virtualName=""
             explicitFocusOrder="0" pos="4 150 8M 154M" sourceFile="SamplerDisplay.cpp"
             constructorParams=""/>
  <LABEL name="volume-label" id="73228f61d5b45b64" memberName="volLabel"
         virtualName="" explicitFocusOrder="0" pos="293R 0 52 36" textCol="ffdadada"
         edTextCol="ff000000" edBkgCol="0" hiliteCol="261151ee" labelText="Volume&#10;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.16e1" kerning="0" bold="0"
         italic="0" justification="34"/>
  <GENERICCOMPONENT name="meter" id="bfda63f9ccd27d97" memberName="meter" virtualName=""
                    explicitFocusOrder="0" pos="142R 11 136 17" class="LevelMeter"
                    params="2, true"/>
  <SLIDER name="volume" id="47a6f6acb9d0b4b3" memberName="volume" virtualName=""
          explicitFocusOrder="0" pos="238R 4 28 30" tooltip="Master Volume"
          bkgcol="0" thumbcol="ffffffff" trackcol="7fffffff" rotarysliderfill="ffffffff"
          rotaryslideroutline="ff2a2a2a" textboxhighlight="66df1010" min="-7e1"
          max="6" int="1e-3" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <GENERICCOMPONENT name="keyboard" id="99aac150ed63807" memberName="keyboard" virtualName=""
                    explicitFocusOrder="0" pos="2 116r 8M 73" class="KeyboardWidget"
                    params="keyboardState"/>
  <LABEL name="OutputLabel" id="277ce632dc5f2ace" memberName="outputLabel"
         virtualName="" explicitFocusOrder="0" pos="196R 0 48 36" textCol="ffdadada"
         edTextCol="ff000000" edBkgCol="0" labelText="Output" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="1.16e1" kerning="0" bold="0" italic="0" justification="34"/>
  <LABEL name="KSP1Label" id="ef55d3420a00b74f" memberName="ksp1Label"
         virtualName="" explicitFocusOrder="0" pos="3 0 196 36" edTextCol="ff000000"
         edBkgCol="0" labelText="KSP-1&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="2e1"
         kerning="4.19e-1" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="SoundsButton" id="50b83eadd71f2375" memberName="soundsButton"
              virtualName="" explicitFocusOrder="0" pos="0Cr 125 36 16" bgColOn="ff1867ae"
              buttonText="Sounds" connectedEdges="2" needsCallback="1" radioGroupId="1"/>
  <TEXTBUTTON name="LayersButton" id="9e9a13f32a730b59" memberName="layersButton"
              virtualName="" explicitFocusOrder="0" pos="1C 125 36 16" bgColOn="ff1867ae"
              buttonText="Layers" connectedEdges="1" needsCallback="1" radioGroupId="1"/>
  <TEXTBUTTON name="EditButton" id="8610fb54d6f3dcde" memberName="editButton"
              virtualName="" explicitFocusOrder="0" pos="4 125 36 16" bgColOn="ff1867ae"
              buttonText="Edit" connectedEdges="0" needsCallback="1" radioGroupId="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif



} /* KSP1 */

//[EndFile] You can add extra defines here...
//[/EndFile]

