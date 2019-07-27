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
    layerKeyButton.reset (new TextButton ("layerKeyButton"));
    addAndMakeVisible (layerKeyButton.get());
    layerKeyButton->setButtonText (TRANS("K"));
    layerKeyButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    layerKeyButton->addListener (this);
    layerKeyButton->setColour (TextButton::buttonColourId, Colour (0xffe4e84f));
    layerKeyButton->setColour (TextButton::buttonOnColourId, Colour (0xfffcab36));

    articulationGroup.reset (new GroupComponent ("articulationGroup",
                                                 String()));
    addAndMakeVisible (articulationGroup.get());
    articulationGroup->setColour (GroupComponent::outlineColourId, Colour (0x00777777));
    articulationGroup->setColour (GroupComponent::textColourId, Colour (0xffcdcdcd));

    volLabel.reset (new Label ("volume-label",
                               TRANS("Master Vol.\n")));
    addAndMakeVisible (volLabel.get());
    volLabel->setFont (Font (11.60f, Font::italic));
    volLabel->setJustificationType (Justification::centredLeft);
    volLabel->setEditable (false, false, false);
    volLabel->setColour (Label::textColourId, Colour (0xffdadada));
    volLabel->setColour (TextEditor::textColourId, Colours::black);
    volLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    volLabel->setColour (TextEditor::highlightColourId, Colour (0x261151ee));

    layerVolume.reset (new Slider ("layerVolume"));
    addAndMakeVisible (layerVolume.get());
    layerVolume->setTooltip (TRANS("Key Volume"));
    layerVolume->setRange (-30, 12, 0.001);
    layerVolume->setSliderStyle (Slider::RotaryVerticalDrag);
    layerVolume->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    layerVolume->setColour (Slider::backgroundColourId, Colour (0x00000000));
    layerVolume->setColour (Slider::trackColourId, Colour (0x7fffffff));
    layerVolume->setColour (Slider::rotarySliderFillColourId, Colour (0x73000000));
    layerVolume->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff2a2a2a));
    layerVolume->addListener (this);

    voiceGroup.reset (new Slider ("voiceGroup"));
    addAndMakeVisible (voiceGroup.get());
    voiceGroup->setTooltip (TRANS("Voice Group"));
    voiceGroup->setRange (-1, 7, 1);
    voiceGroup->setSliderStyle (Slider::IncDecButtons);
    voiceGroup->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    voiceGroup->setColour (Slider::backgroundColourId, Colour (0x00000000));
    voiceGroup->setColour (Slider::trackColourId, Colour (0x7fffffff));
    voiceGroup->setColour (Slider::rotarySliderFillColourId, Colour (0xffaeaeae));
    voiceGroup->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff565656));
    voiceGroup->addListener (this);

    layersLabel.reset (new Label ("LayersLabel",
                                  TRANS("LAYERS")));
    addAndMakeVisible (layersLabel.get());
    layersLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Bold"));
    layersLabel->setJustificationType (Justification::centred);
    layersLabel->setEditable (false, false, false);
    layersLabel->setColour (Label::textColourId, Colour (0xffcdcdcd));
    layersLabel->setColour (TextEditor::textColourId, Colours::black);
    layersLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    layersListBox.reset (new LayersListBox());
    addAndMakeVisible (layersListBox.get());
    layersListBox->setName ("layers-list-box");

    triggerMode.reset (new ComboBox ("triggerMode"));
    addAndMakeVisible (triggerMode.get());
    triggerMode->setTooltip (TRANS("Trigger Mode"));
    triggerMode->setEditableText (false);
    triggerMode->setJustificationType (Justification::centredLeft);
    triggerMode->setTextWhenNothingSelected (TRANS("Trigger Mode"));
    triggerMode->setTextWhenNoChoicesAvailable (TRANS("Trigger Mode"));
    triggerMode->addItem (TRANS("Retrigger"), 1);
    triggerMode->addItem (TRANS("One Shot"), 2);
    triggerMode->addItem (TRANS("Gate"), 3);
    triggerMode->addItem (TRANS("One Shot Gate"), 4);
    triggerMode->addListener (this);

    meter.reset (new LevelMeter());
    addAndMakeVisible (meter.get());
    meter->setName ("meter");

    dbScale.reset (new kv::DecibelScaleComponent());
    addAndMakeVisible (dbScale.get());
    dbScale->setName ("dbScale");

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

    layerPan.reset (new Slider ("layer-pan"));
    addAndMakeVisible (layerPan.get());
    layerPan->setTooltip (TRANS("Panning"));
    layerPan->setRange (0, 1, 0.001);
    layerPan->setSliderStyle (Slider::RotaryVerticalDrag);
    layerPan->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    layerPan->setColour (Slider::backgroundColourId, Colour (0x00000000));
    layerPan->setColour (Slider::trackColourId, Colour (0x7fffffff));
    layerPan->setColour (Slider::rotarySliderFillColourId, Colour (0x73000000));
    layerPan->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff2a2a2a));
    layerPan->addListener (this);

    layerPitch.reset (new Slider ("layer:pitch"));
    addAndMakeVisible (layerPitch.get());
    layerPitch->setTooltip (TRANS("Pitch"));
    layerPitch->setRange (-24, 24, 0);
    layerPitch->setSliderStyle (Slider::RotaryVerticalDrag);
    layerPitch->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    layerPitch->setColour (Slider::backgroundColourId, Colour (0x00000000));
    layerPitch->setColour (Slider::trackColourId, Colour (0x7fffffff));
    layerPitch->setColour (Slider::rotarySliderFillColourId, Colour (0x73000000));
    layerPitch->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff2a2a2a));
    layerPitch->addListener (this);

    fxSend1.reset (new Slider ("key:fx-send-1"));
    addAndMakeVisible (fxSend1.get());
    fxSend1->setTooltip (TRANS("FX Send 1"));
    fxSend1->setRange (0, 1, 0);
    fxSend1->setSliderStyle (Slider::RotaryVerticalDrag);
    fxSend1->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    fxSend1->setColour (Slider::backgroundColourId, Colour (0x00000000));
    fxSend1->setColour (Slider::trackColourId, Colour (0xffeeeeee));
    fxSend1->setColour (Slider::rotarySliderFillColourId, Colour (0x73000000));
    fxSend1->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff2a2a2a));
    fxSend1->addListener (this);

    fxSend2.reset (new Slider ("key:fx-send-2"));
    addAndMakeVisible (fxSend2.get());
    fxSend2->setTooltip (TRANS("FX Send 2"));
    fxSend2->setRange (0, 1, 0);
    fxSend2->setSliderStyle (Slider::RotaryVerticalDrag);
    fxSend2->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    fxSend2->setColour (Slider::backgroundColourId, Colours::black);
    fxSend2->setColour (Slider::trackColourId, Colour (0x7fffffff));
    fxSend2->setColour (Slider::rotarySliderFillColourId, Colour (0x73000000));
    fxSend2->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff2a2a2a));
    fxSend2->addListener (this);

    mediaTabs.reset (new TabbedComponent (TabbedButtonBar::TabsAtTop));
    addAndMakeVisible (mediaTabs.get());
    mediaTabs->setTabBarDepth (25);
    mediaTabs->addTab (TRANS("Asset Library"), Colours::lightgrey, new Component(), true);
    mediaTabs->setCurrentTabIndex (0);

    fxSend3.reset (new Slider ("key:fx-send-1"));
    addAndMakeVisible (fxSend3.get());
    fxSend3->setTooltip (TRANS("FX Send 1"));
    fxSend3->setRange (0, 1, 0);
    fxSend3->setSliderStyle (Slider::RotaryVerticalDrag);
    fxSend3->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    fxSend3->setColour (Slider::backgroundColourId, Colour (0x00000000));
    fxSend3->setColour (Slider::trackColourId, Colour (0x7fffffff));
    fxSend3->setColour (Slider::rotarySliderFillColourId, Colour (0x73000000));
    fxSend3->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff2a2a2a));
    fxSend3->addListener (this);

    fxSend4.reset (new Slider ("key:fxSend4"));
    addAndMakeVisible (fxSend4.get());
    fxSend4->setTooltip (TRANS("FX Send 2"));
    fxSend4->setRange (0, 1, 0);
    fxSend4->setSliderStyle (Slider::RotaryVerticalDrag);
    fxSend4->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    fxSend4->setColour (Slider::backgroundColourId, Colour (0x00000000));
    fxSend4->setColour (Slider::trackColourId, Colour (0x7fffffff));
    fxSend4->setColour (Slider::rotarySliderFillColourId, Colour (0x73000000));
    fxSend4->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff2a2929));
    fxSend4->addListener (this);

    layerVolLabel.reset (new Label ("layerVolLabel",
                                    TRANS("Layer Vol.")));
    addAndMakeVisible (layerVolLabel.get());
    layerVolLabel->setFont (Font (11.60f, Font::italic));
    layerVolLabel->setJustificationType (Justification::centred);
    layerVolLabel->setEditable (false, false, false);
    layerVolLabel->setColour (Label::textColourId, Colour (0xffdadada));
    layerVolLabel->setColour (TextEditor::textColourId, Colours::black);
    layerVolLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    layerVolLabel->setColour (TextEditor::highlightColourId, Colour (0x261151ee));

    layerPanLabel.reset (new Label ("layerPanLabel",
                                    TRANS("Layer Pan")));
    addAndMakeVisible (layerPanLabel.get());
    layerPanLabel->setFont (Font (11.60f, Font::italic));
    layerPanLabel->setJustificationType (Justification::centred);
    layerPanLabel->setEditable (false, false, false);
    layerPanLabel->setColour (Label::textColourId, Colour (0xffdadada));
    layerPanLabel->setColour (TextEditor::textColourId, Colours::black);
    layerPanLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    layerPanLabel->setColour (TextEditor::highlightColourId, Colour (0x261151ee));

    layerPitchLabel.reset (new Label ("layerPitchLabel",
                                      TRANS("Layer Pitch")));
    addAndMakeVisible (layerPitchLabel.get());
    layerPitchLabel->setFont (Font (11.60f, Font::italic));
    layerPitchLabel->setJustificationType (Justification::centred);
    layerPitchLabel->setEditable (false, false, false);
    layerPitchLabel->setColour (Label::textColourId, Colour (0xffdadada));
    layerPitchLabel->setColour (TextEditor::textColourId, Colours::black);
    layerPitchLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    layerPitchLabel->setColour (TextEditor::highlightColourId, Colour (0x261151ee));

    fx1Label.reset (new Label ("fx1Label",
                               TRANS("Aux 1")));
    addAndMakeVisible (fx1Label.get());
    fx1Label->setFont (Font (11.60f, Font::italic));
    fx1Label->setJustificationType (Justification::centredLeft);
    fx1Label->setEditable (false, false, false);
    fx1Label->setColour (Label::textColourId, Colour (0xffdadada));
    fx1Label->setColour (TextEditor::textColourId, Colours::black);
    fx1Label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    fx1Label->setColour (TextEditor::highlightColourId, Colour (0x261151ee));

    fx1Label2.reset (new Label ("fx1Label",
                                TRANS("Aux 2")));
    addAndMakeVisible (fx1Label2.get());
    fx1Label2->setFont (Font (11.60f, Font::italic));
    fx1Label2->setJustificationType (Justification::centredLeft);
    fx1Label2->setEditable (false, false, false);
    fx1Label2->setColour (Label::textColourId, Colour (0xffdadada));
    fx1Label2->setColour (TextEditor::textColourId, Colours::black);
    fx1Label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    fx1Label2->setColour (TextEditor::highlightColourId, Colour (0x261151ee));

    fx1Label3.reset (new Label ("fx1Label",
                                TRANS("Aux 3")));
    addAndMakeVisible (fx1Label3.get());
    fx1Label3->setFont (Font (11.60f, Font::italic));
    fx1Label3->setJustificationType (Justification::centredLeft);
    fx1Label3->setEditable (false, false, false);
    fx1Label3->setColour (Label::textColourId, Colour (0xffdadada));
    fx1Label3->setColour (TextEditor::textColourId, Colours::black);
    fx1Label3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    fx1Label3->setColour (TextEditor::highlightColourId, Colour (0x261151ee));

    fx1Label4.reset (new Label ("fx1Label",
                                TRANS("Aux 4")));
    addAndMakeVisible (fx1Label4.get());
    fx1Label4->setFont (Font (11.60f, Font::italic));
    fx1Label4->setJustificationType (Justification::centredLeft);
    fx1Label4->setEditable (false, false, false);
    fx1Label4->setColour (Label::textColourId, Colour (0xffdadada));
    fx1Label4->setColour (TextEditor::textColourId, Colours::black);
    fx1Label4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    fx1Label4->setColour (TextEditor::highlightColourId, Colour (0x261151ee));

    keyboard.reset (new KeyboardWidget (keyboardState));
    addAndMakeVisible (keyboard.get());
    keyboard->setName ("keyboard");

    display.reset (new SamplerDisplay());
    addAndMakeVisible (display.get());
    keyLength.reset (new Slider ("keyLength"));
    addAndMakeVisible (keyLength.get());
    keyLength->setTooltip (TRANS("Set the keyspan of the selected note(s)"));
    keyLength->setRange (0, 127, 1);
    keyLength->setSliderStyle (Slider::IncDecButtons);
    keyLength->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    keyLength->addListener (this);

    propsButton.reset (new TextButton ("propsButton"));
    addAndMakeVisible (propsButton.get());
    propsButton->setButtonText (TRANS("PROPS"));
    propsButton->addListener (this);
    propsButton->setColour (TextButton::buttonColourId, Colour (0xfffffed9));
    propsButton->setColour (TextButton::buttonOnColourId, Colour (0xfffce828));
    propsButton->setColour (TextButton::textColourOffId, Colours::white);
    propsButton->setColour (TextButton::textColourOnId, Colours::black);

    editButton.reset (new TextButton ("editButton"));
    addAndMakeVisible (editButton.get());
    editButton->setButtonText (TRANS("EDIT"));
    editButton->addListener (this);
    editButton->setColour (TextButton::buttonColourId, Colour (0xffffe9bb));
    editButton->setColour (TextButton::buttonOnColourId, Colour (0xffff9433));
    editButton->setColour (TextButton::textColourOffId, Colours::white);


    //[UserPreSize]
    display->connectUpdateClient (*this);
    keyboard->signalMidi().connect (boost::bind (&SamplerView::onKeyboardMidi, this, ::_1));
    keyboard->signalKeySelected().connect (boost::bind (&SamplerView::onKeySelected, this, ::_1));
    layersListBox->signalSelected().connect (boost::bind (&SamplerView::layerChosen, this));
    //[/UserPreSize]

    setSize (960, 540);


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

    layerKeyButton = nullptr;
    articulationGroup = nullptr;
    volLabel = nullptr;
    layerVolume = nullptr;
    voiceGroup = nullptr;
    layersLabel = nullptr;
    layersListBox = nullptr;
    triggerMode = nullptr;
    meter = nullptr;
    dbScale = nullptr;
    volume = nullptr;
    layerPan = nullptr;
    layerPitch = nullptr;
    fxSend1 = nullptr;
    fxSend2 = nullptr;
    mediaTabs = nullptr;
    fxSend3 = nullptr;
    fxSend4 = nullptr;
    layerVolLabel = nullptr;
    layerPanLabel = nullptr;
    layerPitchLabel = nullptr;
    fx1Label = nullptr;
    fx1Label2 = nullptr;
    fx1Label3 = nullptr;
    fx1Label4 = nullptr;
    keyboard = nullptr;
    display = nullptr;
    keyLength = nullptr;
    propsButton = nullptr;
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

    layerKeyButton->setBounds (188, getHeight() - 179, 18, 18);
    articulationGroup->setBounds ((getWidth() / 2) + -43, getHeight() - 198, 201, 78);
    volLabel->setBounds (getWidth() - 113, getHeight() - 129, 71, 16);
    layerVolume->setBounds (207, getHeight() - 178, 56, 56);
    voiceGroup->setBounds (((getWidth() / 2) + -43) + 103, (getHeight() - 198) + 78 - 24, 85, 13);
    layersLabel->setBounds (4, getHeight() - 152, 172, 24);
    layersListBox->setBounds (4, getHeight() - 132, 172, 128);
    triggerMode->setBounds (((getWidth() / 2) + -43) + 103, (getHeight() - 198) + 78 - 43, 85, 13);
    meter->setBounds (getWidth() - 28, getHeight() - 183, 20, 177);
    dbScale->setBounds (getWidth() - 29, getHeight() - 4 - 179, 21, 179);
    volume->setBounds (getWidth() - 112, getHeight() - 177, 61, 58);
    layerPan->setBounds (273, getHeight() - 162, 56, 56);
    layerPitch->setBounds (336, getHeight() - 178, 56, 56);
    fxSend1->setBounds (getWidth() - 283, getHeight() - 154, 40, 40);
    fxSend2->setBounds (getWidth() - 244, getHeight() - 176, 40, 40);
    mediaTabs->setBounds (4, 4, 172, getHeight() - 162);
    fxSend3->setBounds (getWidth() - 204, getHeight() - 155, 40, 40);
    fxSend4->setBounds (getWidth() - 164, getHeight() - 173, 40, 40);
    layerVolLabel->setBounds (206, getHeight() - 128, 58, 15);
    layerPanLabel->setBounds (274, getHeight() - 177, 58, 15);
    layerPitchLabel->setBounds (336, getHeight() - 126, 58, 15);
    fx1Label->setBounds (getWidth() - 278, getHeight() - 170, 31, 18);
    fx1Label2->setBounds (getWidth() - 238, getHeight() - 144, 31, 18);
    fx1Label3->setBounds (getWidth() - 200, getHeight() - 170, 31, 18);
    fx1Label4->setBounds (getWidth() - 159, getHeight() - 141, 31, 18);
    keyboard->setBounds (187, getHeight() - 4 - 101, getWidth() - 223, 101);
    display->setBounds (186, 27, getWidth() - 188, getHeight() - 212);
    keyLength->setBounds (((getWidth() / 2) + -43) + 201 / 2 + -84, (getHeight() - 198) + 78 - 25, 80, 13);
    propsButton->setBounds (getWidth() - 55, 4, 49, 18);
    editButton->setBounds (getWidth() - 107, 4, 49, 18);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SamplerView::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == layerKeyButton.get())
    {
        //[UserButtonCode_layerKeyButton] -- add your button handler code here..
        //[/UserButtonCode_layerKeyButton]
    }
    else if (buttonThatWasClicked == propsButton.get())
    {
        //[UserButtonCode_propsButton] -- add your button handler code here..
        //[/UserButtonCode_propsButton]
    }
    else if (buttonThatWasClicked == editButton.get())
    {
        //[UserButtonCode_editButton] -- add your button handler code here..
        //[/UserButtonCode_editButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void SamplerView::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == layerVolume.get())
    {
        //[UserSliderCode_layerVolume] -- add your slider handling code here..
        //[/UserSliderCode_layerVolume]
    }
    else if (sliderThatWasMoved == voiceGroup.get())
    {
        //[UserSliderCode_voiceGroup] -- add your slider handling code here..
        //[/UserSliderCode_voiceGroup]
    }
    else if (sliderThatWasMoved == volume.get())
    {
        //[UserSliderCode_volume] -- add your slider handling code here..
        //[/UserSliderCode_volume]
    }
    else if (sliderThatWasMoved == layerPan.get())
    {
        //[UserSliderCode_layerPan] -- add your slider handling code here..
        //[/UserSliderCode_layerPan]
    }
    else if (sliderThatWasMoved == layerPitch.get())
    {
        //[UserSliderCode_layerPitch] -- add your slider handling code here..
        //[/UserSliderCode_layerPitch]
    }
    else if (sliderThatWasMoved == fxSend1.get())
    {
        //[UserSliderCode_fxSend1] -- add your slider handling code here..
        //[/UserSliderCode_fxSend1]
    }
    else if (sliderThatWasMoved == fxSend2.get())
    {
        //[UserSliderCode_fxSend2] -- add your slider handling code here..
        //[/UserSliderCode_fxSend2]
    }
    else if (sliderThatWasMoved == fxSend3.get())
    {
        //[UserSliderCode_fxSend3] -- add your slider handling code here..
        //[/UserSliderCode_fxSend3]
    }
    else if (sliderThatWasMoved == fxSend4.get())
    {
        //[UserSliderCode_fxSend4] -- add your slider handling code here..
        //[/UserSliderCode_fxSend4]
    }
    else if (sliderThatWasMoved == keyLength.get())
    {
        //[UserSliderCode_keyLength] -- add your slider handling code here..
        //[/UserSliderCode_keyLength]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void SamplerView::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == triggerMode.get())
    {
        //[UserComboBoxCode_triggerMode] -- add your combo box handling code here..
        //[/UserComboBoxCode_triggerMode]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void SamplerView::buttonStateChanged (Button* btn)
{

}

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
    display->selectNote (k, true);
    const KeyItem item (display->selectedKey());
    layersListBox->setKeyItem (item);
    updateControls (dontSendNotification);
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
    LayerItem layer (layersListBox->getSelectedLayer());
    KeyItem key (instrument->getKey (layer.getNote()));

    // instrument controls
    volume->getValueObject().referTo (instrument->getPropertyAsValue (Tags::volume));
    const bool isControllingLayer = layerKeyButton->getToggleState();

    if (isControllingLayer)
    {
        layerVolLabel->setText ("Vol.", n);
        layerPitchLabel->setText ("Pitch", n);
        layerPanLabel->setText ("Pan", n);
    }
    else
    {
        layerVolLabel->setText ("Vol.", n);
        layerPitchLabel->setText ("Pitch", n);
        layerPanLabel->setText ("Pan", n);
    }

    // key controls
    if (key.isValid())
    {
        voiceGroup->getValueObject().referTo (key.getPropertyAsValue (Tags::voiceGroup));
        triggerMode->setSelectedItemIndex (key.getTriggerMode(), n);

        if (! isControllingLayer)
        {
            layerVolume->getValueObject().referTo (key.getPropertyAsValue (kv::Slugs::volume));
            layerPan->getValueObject().referTo (key.getPropertyAsValue (Tags::panning));
            layerPitch->getValueObject().referTo (key.getPropertyAsValue (kv::Slugs::pitch));
        }
        /* fxSend1->setValue (key.node().getProperty (KSP1::fxSendIdentifier(1)), n);
        fxSend2->setValue (key.node().getProperty (KSP1::fxSendIdentifier(2)), n);
        keyLength->setValue (key.node().getProperty (Slugs::length, 0.0));
        attack->setValue ((double) key.node().getProperty (Slugs::attack), n);
        decay->setValue ((double) key.node().getProperty (Slugs::decay), n);
        sustain->setValue ((double) key.node().getProperty (Slugs::sustain), n);
        release->setValue ((double) key.node().getProperty (Slugs::release), n); */

    }

    if (layer.isValid() && isControllingLayer)
    {
        layerVolume->getValueObject().referTo (layer.getPropertyAsValue (kv::Slugs::volume));
        layerPan->getValueObject().referTo (layer.getPropertyAsValue (Tags::panning));
        layerPitch->getValueObject().referTo (layer.getPropertyAsValue (kv::Slugs::pitch));
        /* layerCutoff->setValue (layer.cutoff(), n);
        layerResonance->setValue (layer.resonance(), n);
        velocityRange->setMaxValue ((double) layer.getProperty (KSP1::velocityUpper) * 127.0f, n);
        velocityRange->setMinValue ((double) layer.getProperty (KSP1::velocityLower) * 127.0f, n); */
    }

}

InstrumentPtr SamplerView::getInstrument (const int) const
{
    return display->getInstrument();
}

void SamplerView::setInstrument (InstrumentPtr i)
{
    display->setInstrument (i);
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
                 componentName="samplerView" parentClasses="public Panel, public DragAndDropContainer"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="0"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="960"
                 initialHeight="540">
  <BACKGROUND backgroundColour="ff8c3131">
    <RECT pos="0 0 100% 100%" fill="linear: 285 2, 283 651, 0=ff747474, 1=ff353535"
          hasStroke="0"/>
  </BACKGROUND>
  <TEXTBUTTON name="layerKeyButton" id="db4a5e029bc91d2" memberName="layerKeyButton"
              virtualName="" explicitFocusOrder="0" pos="188 179R 18 18" bgColOff="ffe4e84f"
              bgColOn="fffcab36" buttonText="K" connectedEdges="15" needsCallback="1"
              radioGroupId="0"/>
  <GROUPCOMPONENT name="articulationGroup" id="5fe3d96c50772121" memberName="articulationGroup"
                  virtualName="" explicitFocusOrder="0" pos="-43C 198R 201 78"
                  outlinecol="777777" textcol="ffcdcdcd" title=""/>
  <LABEL name="volume-label" id="73228f61d5b45b64" memberName="volLabel"
         virtualName="" explicitFocusOrder="0" pos="113R 129R 71 16" textCol="ffdadada"
         edTextCol="ff000000" edBkgCol="0" hiliteCol="261151ee" labelText="Master Vol.&#10;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.16e1" kerning="0" bold="0"
         italic="1" justification="33" typefaceStyle="Italic"/>
  <SLIDER name="layerVolume" id="bf65c3b220f44da5" memberName="layerVolume"
          virtualName="" explicitFocusOrder="0" pos="207 178R 56 56" tooltip="Key Volume"
          bkgcol="0" trackcol="7fffffff" rotarysliderfill="73000000" rotaryslideroutline="ff2a2a2a"
          min="-3e1" max="1.2e1" int="1e-3" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="voiceGroup" id="947ac3f3088f71a6" memberName="voiceGroup"
          virtualName="" explicitFocusOrder="0" pos="103 24R 85 13" posRelativeX="5fe3d96c50772121"
          posRelativeY="5fe3d96c50772121" tooltip="Voice Group" bkgcol="0"
          trackcol="7fffffff" rotarysliderfill="ffaeaeae" rotaryslideroutline="ff565656"
          min="-1" max="7" int="1" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="LayersLabel" id="b92c4c02f15c9879" memberName="layersLabel"
         virtualName="" explicitFocusOrder="0" pos="4 152R 172 24" textCol="ffcdcdcd"
         edTextCol="ff000000" edBkgCol="0" labelText="LAYERS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="1.5e1" kerning="0" bold="1" italic="0" justification="36"
         typefaceStyle="Bold"/>
  <GENERICCOMPONENT name="layers-list-box" id="8a15718f78e152e0" memberName="layersListBox"
                    virtualName="" explicitFocusOrder="0" pos="4 132R 172 128" class="LayersListBox"
                    params=""/>
  <COMBOBOX name="triggerMode" id="9de3dfb17cdff6fe" memberName="triggerMode"
            virtualName="" explicitFocusOrder="0" pos="103 43R 85 13" posRelativeX="5fe3d96c50772121"
            posRelativeY="5fe3d96c50772121" tooltip="Trigger Mode" editable="0"
            layout="33" items="Retrigger&#10;One Shot&#10;Gate&#10;One Shot Gate"
            textWhenNonSelected="Trigger Mode" textWhenNoItems="Trigger Mode"/>
  <GENERICCOMPONENT name="meter" id="bfda63f9ccd27d97" memberName="meter" virtualName=""
                    explicitFocusOrder="0" pos="28R 183R 20 177" class="LevelMeter"
                    params=""/>
  <GENERICCOMPONENT name="dbScale" id="7c019624d677326b" memberName="dbScale" virtualName=""
                    explicitFocusOrder="0" pos="29R 4Rr 21 179" class="kv::DecibelScaleComponent"
                    params=""/>
  <SLIDER name="volume" id="47a6f6acb9d0b4b3" memberName="volume" virtualName=""
          explicitFocusOrder="0" pos="112R 177R 61 58" tooltip="Master Volume"
          bkgcol="0" trackcol="7fffffff" rotarysliderfill="ff5591ad" rotaryslideroutline="ff2a2a2a"
          min="-7e1" max="6" int="1e-3" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="layer-pan" id="1de5ef00d853a1b0" memberName="layerPan"
          virtualName="" explicitFocusOrder="0" pos="273 162R 56 56" tooltip="Panning"
          bkgcol="0" trackcol="7fffffff" rotarysliderfill="73000000" rotaryslideroutline="ff2a2a2a"
          min="0" max="1" int="1e-3" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="layer:pitch" id="86fb2e7a1567e16" memberName="layerPitch"
          virtualName="" explicitFocusOrder="0" pos="336 178R 56 56" tooltip="Pitch"
          bkgcol="0" trackcol="7fffffff" rotarysliderfill="73000000" rotaryslideroutline="ff2a2a2a"
          min="-2.4e1" max="2.4e1" int="0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="key:fx-send-1" id="19fcb4fb69e6ec74" memberName="fxSend1"
          virtualName="" explicitFocusOrder="0" pos="283R 154R 40 40" tooltip="FX Send 1"
          bkgcol="0" trackcol="ffeeeeee" rotarysliderfill="73000000" rotaryslideroutline="ff2a2a2a"
          min="0" max="1" int="0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="key:fx-send-2" id="69422f84d84ad59c" memberName="fxSend2"
          virtualName="" explicitFocusOrder="0" pos="244R 176R 40 40" tooltip="FX Send 2"
          bkgcol="ff000000" trackcol="7fffffff" rotarysliderfill="73000000"
          rotaryslideroutline="ff2a2a2a" min="0" max="1" int="0" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <TABBEDCOMPONENT name="mediaTabs" id="8dd080e8355410ba" memberName="mediaTabs"
                   virtualName="" explicitFocusOrder="0" pos="4 4 172 162M" orientation="top"
                   tabBarDepth="25" initialTab="0">
    <TAB name="Asset Library" colour="ffd3d3d3" useJucerComp="0" contentClassName="Component"
         constructorParams="" jucerComponentFile=""/>
  </TABBEDCOMPONENT>
  <SLIDER name="key:fx-send-1" id="1fde028392ea9574" memberName="fxSend3"
          virtualName="" explicitFocusOrder="0" pos="204R 155R 40 40" tooltip="FX Send 1"
          bkgcol="0" trackcol="7fffffff" rotarysliderfill="73000000" rotaryslideroutline="ff2a2a2a"
          min="0" max="1" int="0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="key:fxSend4" id="d533f517dda027c3" memberName="fxSend4"
          virtualName="" explicitFocusOrder="0" pos="164R 173R 40 40" tooltip="FX Send 2"
          bkgcol="0" trackcol="7fffffff" rotarysliderfill="73000000" rotaryslideroutline="ff2a2929"
          min="0" max="1" int="0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="layerVolLabel" id="4c9d302bf2827bcb" memberName="layerVolLabel"
         virtualName="" explicitFocusOrder="0" pos="206 128R 58 15" textCol="ffdadada"
         edTextCol="ff000000" edBkgCol="0" hiliteCol="261151ee" labelText="Layer Vol."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.16e1" kerning="0" bold="0"
         italic="1" justification="36" typefaceStyle="Italic"/>
  <LABEL name="layerPanLabel" id="aad53eaeda5c6183" memberName="layerPanLabel"
         virtualName="" explicitFocusOrder="0" pos="274 177R 58 15" textCol="ffdadada"
         edTextCol="ff000000" edBkgCol="0" hiliteCol="261151ee" labelText="Layer Pan"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.16e1" kerning="0" bold="0"
         italic="1" justification="36" typefaceStyle="Italic"/>
  <LABEL name="layerPitchLabel" id="9bd479de36829fb4" memberName="layerPitchLabel"
         virtualName="" explicitFocusOrder="0" pos="336 126R 58 15" textCol="ffdadada"
         edTextCol="ff000000" edBkgCol="0" hiliteCol="261151ee" labelText="Layer Pitch"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.16e1" kerning="0" bold="0"
         italic="1" justification="36" typefaceStyle="Italic"/>
  <LABEL name="fx1Label" id="157d12cf7acd6a0d" memberName="fx1Label" virtualName=""
         explicitFocusOrder="0" pos="278R 170R 31 18" textCol="ffdadada"
         edTextCol="ff000000" edBkgCol="0" hiliteCol="261151ee" labelText="Aux 1"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.16e1" kerning="0" bold="0"
         italic="1" justification="33" typefaceStyle="Italic"/>
  <LABEL name="fx1Label" id="672db88d55e40cd9" memberName="fx1Label2"
         virtualName="" explicitFocusOrder="0" pos="238R 144R 31 18" textCol="ffdadada"
         edTextCol="ff000000" edBkgCol="0" hiliteCol="261151ee" labelText="Aux 2"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.16e1" kerning="0" bold="0"
         italic="1" justification="33" typefaceStyle="Italic"/>
  <LABEL name="fx1Label" id="278b158d0bb943f9" memberName="fx1Label3"
         virtualName="" explicitFocusOrder="0" pos="200R 170R 31 18" textCol="ffdadada"
         edTextCol="ff000000" edBkgCol="0" hiliteCol="261151ee" labelText="Aux 3"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.16e1" kerning="0" bold="0"
         italic="1" justification="33" typefaceStyle="Italic"/>
  <LABEL name="fx1Label" id="71864d4f9ce36d52" memberName="fx1Label4"
         virtualName="" explicitFocusOrder="0" pos="159R 141R 31 18" textCol="ffdadada"
         edTextCol="ff000000" edBkgCol="0" hiliteCol="261151ee" labelText="Aux 4"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.16e1" kerning="0" bold="0"
         italic="1" justification="33" typefaceStyle="Italic"/>
  <GENERICCOMPONENT name="keyboard" id="99aac150ed63807" memberName="keyboard" virtualName=""
                    explicitFocusOrder="0" pos="187 4Rr 223M 101" class="KeyboardWidget"
                    params="keyboardState"/>
  <JUCERCOMP name="display" id="56c24b8a829e534f" memberName="display" virtualName=""
             explicitFocusOrder="0" pos="186 27 188M 212M" sourceFile="SamplerDisplay.cpp"
             constructorParams=""/>
  <SLIDER name="keyLength" id="a09d8e14cc07f187" memberName="keyLength"
          virtualName="" explicitFocusOrder="0" pos="-84C 25R 80 13" posRelativeX="5fe3d96c50772121"
          posRelativeY="5fe3d96c50772121" tooltip="Set the keyspan of the selected note(s)"
          min="0" max="1.27e2" int="1" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <TEXTBUTTON name="propsButton" id="3e804b5eb108fcd1" memberName="propsButton"
              virtualName="" explicitFocusOrder="0" pos="55R 4 49 18" bgColOff="fffffed9"
              bgColOn="fffce828" textCol="ffffffff" textColOn="ff000000" buttonText="PROPS"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="editButton" id="8f50f14524c81d76" memberName="editButton"
              virtualName="" explicitFocusOrder="0" pos="107R 4 49 18" bgColOff="ffffe9bb"
              bgColOn="ffff9433" textCol="ffffffff" buttonText="EDIT" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif



} /* KSP1 */

//[EndFile] You can add extra defines here...
//[/EndFile]

