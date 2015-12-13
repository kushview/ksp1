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
#include <boost/bind.hpp>

#include "Instrument.h"
#include "SamplerInterface.h"

#include "editor/AssetsListBox.h"
#include "editor/Banks.h"
#include "editor/LevelMeter.h"
#include "editor/SamplerDisplay.h"
#include "editor/EditScreen.h"
#include "editor/ProgramsListBox.h"
#include "editor/LayersListBox.h"
//[/Headers]

#include "SamplerView.h"


namespace KSP1 {
namespace Gui {

//[MiscUserDefs]

// right now this is just a timer that keeps the level meter going
// onDisplayUpdate will probably go away
class SamplerView::Updater :  public Timer
{
public:
    Updater (SamplerView& v) : view (v) { }
    void timerCallback() { view.onDisplayUpdate(); }

private:
    SamplerView& view;
};

//[/MiscUserDefs]

SamplerView::SamplerView ()
{
    setName ("samplerView");
    addAndMakeVisible (layerKeyButton = new TextButton ("layerKeyButton"));
    layerKeyButton->setButtonText (TRANS("K"));
    layerKeyButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    layerKeyButton->addListener (this);
    layerKeyButton->setColour (TextButton::buttonColourId, Colour (0xffe4e84f));
    layerKeyButton->setColour (TextButton::buttonOnColourId, Colour (0xfffcab36));

    addAndMakeVisible (articulationGroup = new GroupComponent ("articulationGroup",
                                                               String::empty));
    articulationGroup->setColour (GroupComponent::outlineColourId, Colour (0x00777777));
    articulationGroup->setColour (GroupComponent::textColourId, Colour (0xffcdcdcd));

    addAndMakeVisible (volLabel = new Label ("volume-label",
                                             TRANS("Master Vol.\n")));
    volLabel->setFont (Font (11.60f, Font::italic));
    volLabel->setJustificationType (Justification::centredLeft);
    volLabel->setEditable (false, false, false);
    volLabel->setColour (Label::textColourId, Colour (0xffdadada));
    volLabel->setColour (TextEditor::textColourId, Colours::black);
    volLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    volLabel->setColour (TextEditor::highlightColourId, Colour (0x261151ee));

    addAndMakeVisible (layerVolume = new Slider ("layerVolume"));
    layerVolume->setTooltip (TRANS("Key Volume"));
    layerVolume->setRange (-30, 12, 0.001);
    layerVolume->setSliderStyle (Slider::RotaryVerticalDrag);
    layerVolume->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    layerVolume->setColour (Slider::backgroundColourId, Colour (0x00000000));
    layerVolume->setColour (Slider::trackColourId, Colour (0x7fffffff));
    layerVolume->setColour (Slider::rotarySliderFillColourId, Colour (0x73000000));
    layerVolume->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff2a2a2a));
    layerVolume->addListener (this);

    addAndMakeVisible (voiceGroup = new Slider ("voiceGroup"));
    voiceGroup->setTooltip (TRANS("Voice Group"));
    voiceGroup->setRange (-1, 7, 1);
    voiceGroup->setSliderStyle (Slider::IncDecButtons);
    voiceGroup->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    voiceGroup->setColour (Slider::backgroundColourId, Colour (0x00000000));
    voiceGroup->setColour (Slider::trackColourId, Colour (0x7fffffff));
    voiceGroup->setColour (Slider::rotarySliderFillColourId, Colour (0xffaeaeae));
    voiceGroup->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff565656));
    voiceGroup->addListener (this);

    addAndMakeVisible (layersLabel = new Label ("LayersLabel",
                                                TRANS("LAYERS")));
    layersLabel->setFont (Font (15.00f, Font::bold));
    layersLabel->setJustificationType (Justification::centred);
    layersLabel->setEditable (false, false, false);
    layersLabel->setColour (Label::textColourId, Colour (0xffcdcdcd));
    layersLabel->setColour (TextEditor::textColourId, Colours::black);
    layersLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (layersListBox = new LayersListBox());
    layersListBox->setName ("layers-list-box");

    addAndMakeVisible (triggerMode = new ComboBox ("triggerMode"));
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

    addAndMakeVisible (meter = new LevelMeter());
    meter->setName ("meter");

    addAndMakeVisible (dbScale = new DecibelScaleComponent());
    dbScale->setName ("dbScale");

    addAndMakeVisible (volume = new Slider ("volume"));
    volume->setTooltip (TRANS("Master Volume"));
    volume->setRange (-70, 6, 0.001);
    volume->setSliderStyle (Slider::RotaryVerticalDrag);
    volume->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    volume->setColour (Slider::backgroundColourId, Colour (0x00000000));
    volume->setColour (Slider::trackColourId, Colour (0x7fffffff));
    volume->setColour (Slider::rotarySliderFillColourId, Colour (0xff5591ad));
    volume->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff2a2a2a));
    volume->addListener (this);

    addAndMakeVisible (layerPan = new Slider ("layer-pan"));
    layerPan->setTooltip (TRANS("Panning"));
    layerPan->setRange (0, 1, 0.001);
    layerPan->setSliderStyle (Slider::RotaryVerticalDrag);
    layerPan->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    layerPan->setColour (Slider::backgroundColourId, Colour (0x00000000));
    layerPan->setColour (Slider::trackColourId, Colour (0x7fffffff));
    layerPan->setColour (Slider::rotarySliderFillColourId, Colour (0x73000000));
    layerPan->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff2a2a2a));
    layerPan->addListener (this);

    addAndMakeVisible (layerPitch = new Slider ("layer:pitch"));
    layerPitch->setTooltip (TRANS("Pitch"));
    layerPitch->setRange (-24, 24, 0);
    layerPitch->setSliderStyle (Slider::RotaryVerticalDrag);
    layerPitch->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    layerPitch->setColour (Slider::backgroundColourId, Colour (0x00000000));
    layerPitch->setColour (Slider::trackColourId, Colour (0x7fffffff));
    layerPitch->setColour (Slider::rotarySliderFillColourId, Colour (0x73000000));
    layerPitch->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff2a2a2a));
    layerPitch->addListener (this);

    addAndMakeVisible (fxSend1 = new Slider ("key:fx-send-1"));
    fxSend1->setTooltip (TRANS("FX Send 1"));
    fxSend1->setRange (0, 1, 0);
    fxSend1->setSliderStyle (Slider::RotaryVerticalDrag);
    fxSend1->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    fxSend1->setColour (Slider::backgroundColourId, Colour (0x00000000));
    fxSend1->setColour (Slider::trackColourId, Colour (0xffeeeeee));
    fxSend1->setColour (Slider::rotarySliderFillColourId, Colour (0x73000000));
    fxSend1->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff2a2a2a));
    fxSend1->addListener (this);

    addAndMakeVisible (fxSend2 = new Slider ("key:fx-send-2"));
    fxSend2->setTooltip (TRANS("FX Send 2"));
    fxSend2->setRange (0, 1, 0);
    fxSend2->setSliderStyle (Slider::RotaryVerticalDrag);
    fxSend2->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    fxSend2->setColour (Slider::backgroundColourId, Colours::black);
    fxSend2->setColour (Slider::trackColourId, Colour (0x7fffffff));
    fxSend2->setColour (Slider::rotarySliderFillColourId, Colour (0x73000000));
    fxSend2->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff2a2a2a));
    fxSend2->addListener (this);

    addAndMakeVisible (mediaTabs = new TabbedComponent (TabbedButtonBar::TabsAtTop));
    mediaTabs->setTabBarDepth (25);
    mediaTabs->addTab (TRANS("Asset Library"), Colours::lightgrey, new AssetsListBox(), true);
    mediaTabs->setCurrentTabIndex (0);

    addAndMakeVisible (articulationControls = new ArticulationControls());
    addAndMakeVisible (fxSend3 = new Slider ("key:fx-send-1"));
    fxSend3->setTooltip (TRANS("FX Send 1"));
    fxSend3->setRange (0, 1, 0);
    fxSend3->setSliderStyle (Slider::RotaryVerticalDrag);
    fxSend3->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    fxSend3->setColour (Slider::backgroundColourId, Colour (0x00000000));
    fxSend3->setColour (Slider::trackColourId, Colour (0x7fffffff));
    fxSend3->setColour (Slider::rotarySliderFillColourId, Colour (0x73000000));
    fxSend3->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff2a2a2a));
    fxSend3->addListener (this);

    addAndMakeVisible (fxSend4 = new Slider ("key:fxSend4"));
    fxSend4->setTooltip (TRANS("FX Send 2"));
    fxSend4->setRange (0, 1, 0);
    fxSend4->setSliderStyle (Slider::RotaryVerticalDrag);
    fxSend4->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    fxSend4->setColour (Slider::backgroundColourId, Colour (0x00000000));
    fxSend4->setColour (Slider::trackColourId, Colour (0x7fffffff));
    fxSend4->setColour (Slider::rotarySliderFillColourId, Colour (0x73000000));
    fxSend4->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff2a2929));
    fxSend4->addListener (this);

    addAndMakeVisible (layerVolLabel = new Label ("layerVolLabel",
                                                  TRANS("Layer Vol.")));
    layerVolLabel->setFont (Font (11.60f, Font::italic));
    layerVolLabel->setJustificationType (Justification::centred);
    layerVolLabel->setEditable (false, false, false);
    layerVolLabel->setColour (Label::textColourId, Colour (0xffdadada));
    layerVolLabel->setColour (TextEditor::textColourId, Colours::black);
    layerVolLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    layerVolLabel->setColour (TextEditor::highlightColourId, Colour (0x261151ee));

    addAndMakeVisible (layerPanLabel = new Label ("layerPanLabel",
                                                  TRANS("Layer Pan")));
    layerPanLabel->setFont (Font (11.60f, Font::italic));
    layerPanLabel->setJustificationType (Justification::centred);
    layerPanLabel->setEditable (false, false, false);
    layerPanLabel->setColour (Label::textColourId, Colour (0xffdadada));
    layerPanLabel->setColour (TextEditor::textColourId, Colours::black);
    layerPanLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    layerPanLabel->setColour (TextEditor::highlightColourId, Colour (0x261151ee));

    addAndMakeVisible (layerPitchLabel = new Label ("layerPitchLabel",
                                                    TRANS("Layer Pitch")));
    layerPitchLabel->setFont (Font (11.60f, Font::italic));
    layerPitchLabel->setJustificationType (Justification::centred);
    layerPitchLabel->setEditable (false, false, false);
    layerPitchLabel->setColour (Label::textColourId, Colour (0xffdadada));
    layerPitchLabel->setColour (TextEditor::textColourId, Colours::black);
    layerPitchLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    layerPitchLabel->setColour (TextEditor::highlightColourId, Colour (0x261151ee));

    addAndMakeVisible (fx1Label = new Label ("fx1Label",
                                             TRANS("Aux 1")));
    fx1Label->setFont (Font (11.60f, Font::italic));
    fx1Label->setJustificationType (Justification::centredLeft);
    fx1Label->setEditable (false, false, false);
    fx1Label->setColour (Label::textColourId, Colour (0xffdadada));
    fx1Label->setColour (TextEditor::textColourId, Colours::black);
    fx1Label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    fx1Label->setColour (TextEditor::highlightColourId, Colour (0x261151ee));

    addAndMakeVisible (fx1Label2 = new Label ("fx1Label",
                                              TRANS("Aux 2")));
    fx1Label2->setFont (Font (11.60f, Font::italic));
    fx1Label2->setJustificationType (Justification::centredLeft);
    fx1Label2->setEditable (false, false, false);
    fx1Label2->setColour (Label::textColourId, Colour (0xffdadada));
    fx1Label2->setColour (TextEditor::textColourId, Colours::black);
    fx1Label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    fx1Label2->setColour (TextEditor::highlightColourId, Colour (0x261151ee));

    addAndMakeVisible (fx1Label3 = new Label ("fx1Label",
                                              TRANS("Aux 3")));
    fx1Label3->setFont (Font (11.60f, Font::italic));
    fx1Label3->setJustificationType (Justification::centredLeft);
    fx1Label3->setEditable (false, false, false);
    fx1Label3->setColour (Label::textColourId, Colour (0xffdadada));
    fx1Label3->setColour (TextEditor::textColourId, Colours::black);
    fx1Label3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    fx1Label3->setColour (TextEditor::highlightColourId, Colour (0x261151ee));

    addAndMakeVisible (fx1Label4 = new Label ("fx1Label",
                                              TRANS("Aux 4")));
    fx1Label4->setFont (Font (11.60f, Font::italic));
    fx1Label4->setJustificationType (Justification::centredLeft);
    fx1Label4->setEditable (false, false, false);
    fx1Label4->setColour (Label::textColourId, Colour (0xffdadada));
    fx1Label4->setColour (TextEditor::textColourId, Colours::black);
    fx1Label4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    fx1Label4->setColour (TextEditor::highlightColourId, Colour (0x261151ee));

    addAndMakeVisible (keyboard = new KeyboardWidget (keyboardState));
    keyboard->setName ("keyboard");

    addAndMakeVisible (display = new SamplerDisplay());
    addAndMakeVisible (keyLength = new Slider ("keyLength"));
    keyLength->setTooltip (TRANS("Set the keyspan of the selected note(s)"));
    keyLength->setRange (0, 127, 1);
    keyLength->setSliderStyle (Slider::IncDecButtons);
    keyLength->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    keyLength->addListener (this);

    addAndMakeVisible (propsButton = new TextButton ("propsButton"));
    propsButton->setButtonText (TRANS("PROPS"));
    propsButton->addListener (this);
    propsButton->setColour (TextButton::buttonColourId, Colour (0xfffffed9));
    propsButton->setColour (TextButton::buttonOnColourId, Colour (0xfffce828));
    propsButton->setColour (TextButton::textColourOnId, Colours::white);
    propsButton->setColour (TextButton::textColourOffId, Colours::black);

    addAndMakeVisible (editButton = new TextButton ("editButton"));
    editButton->setButtonText (TRANS("EDIT"));
    editButton->addListener (this);
    editButton->setColour (TextButton::buttonColourId, Colour (0xffffe9bb));
    editButton->setColour (TextButton::buttonOnColourId, Colour (0xffff9433));
    editButton->setColour (TextButton::textColourOnId, Colours::white);


    //[UserPreSize]

    display->connectUpdateClient (*this);
    keyboard->signalMidi().connect (boost::bind (&SamplerView::onKeyboardMidi, this, ::_1));
    keyboard->signalKeySelected().connect (boost::bind (&SamplerView::onKeySelected, this, ::_1));

    if (ProgramsListBox* box = dynamic_cast<ProgramsListBox*> (mediaTabs->getTabContentComponent (1)))
        box->signalEngaged().connect (boost::bind (&SamplerView::loadProgram, this, ::_1));

    layersListBox->signalSelected().connect (boost::bind (&SamplerView::layerChosen, this));
    //[/UserPreSize]

    setSize (960, 540);


    //[Constructor]

    display->setScreen (Screen::kitScreen);
    volume->setRange (-70.0f, 12.0f);

    // Articulation init
    articulationControls->setVisible (false);

    updater = new Updater (*this);
    updater->startTimer (66);

    //FIXME: Using an asset tree is not needed or wanted
    tree = new AssetTree ("Assets");
    if (AssetsListBox* assets = dynamic_cast<AssetsListBox*> (mediaTabs->getTabContentComponent (0)))
        assets->setRootItem (tree->root());
    //[/Constructor]
}

SamplerView::~SamplerView()
{
    //[Destructor_pre]
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
    articulationControls = nullptr;
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


    //[Destructor]
    //[/Destructor]
}

void SamplerView::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff8c3131));

    g.setGradientFill (ColourGradient (Colour (0xff747474),
                                       285.0f, 2.0f,
                                       Colour (0xff353535),
                                       283.0f, 651.0f,
                                       false));
    g.fillRect (0, 0, proportionOfWidth (1.0000f), proportionOfHeight (1.0000f));

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
    articulationControls->setBounds (((getWidth() / 2) + -43) + 14, (getHeight() - 198) + 34, 85, 13);
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

    if (buttonThatWasClicked == layerKeyButton)
    {
        //[UserButtonCode_layerKeyButton] -- add your button handler code here..
        layerKeyButton->setToggleState (! layerKeyButton->getToggleState(), dontSendNotification);
        if (layerKeyButton->getToggleState())
            layerKeyButton->setButtonText ("L");
        else
            layerKeyButton->setButtonText ("K");
        updateControls();
        //[/UserButtonCode_layerKeyButton]
    }
    else if (buttonThatWasClicked == propsButton)
    {
        //[UserButtonCode_propsButton] -- add your button handler code here..
        if (! propsButton->getToggleState())
        {
            propsButton->setToggleState (true, dontSendNotification);
            editButton->setToggleState (false, dontSendNotification);
            display->setScreen (Screen::kitScreen);
            display->selectNote (display->selectedNote(), true);
        }
        //[/UserButtonCode_propsButton]
    }
    else if (buttonThatWasClicked == editButton)
    {
        //[UserButtonCode_editButton] -- add your button handler code here..
        if (! editButton->getToggleState())
        {
            propsButton->setToggleState (false, dontSendNotification);
            editButton->setToggleState (true, dontSendNotification);
            display->setScreen (Screen::editScreen);
            display->selectNote (display->selectedNote(), true);
        }
        //[/UserButtonCode_editButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void SamplerView::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    InstrumentPtr inst (display->getInstrument());
    LayerItem layer (layersListBox->getSelectedLayer());
    KeyItem key (inst->getKey (layer.getNote ()));
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == layerVolume)
    {
        //[UserSliderCode_layerVolume] -- add your slider handling code here..
        //[/UserSliderCode_layerVolume]
    }
    else if (sliderThatWasMoved == voiceGroup)
    {
        //[UserSliderCode_voiceGroup] -- add your slider handling code here..
        //[/UserSliderCode_voiceGroup]
    }
    else if (sliderThatWasMoved == volume)
    {
        //[UserSliderCode_volume] -- add your slider handling code here..
        //[/UserSliderCode_volume]
    }
    else if (sliderThatWasMoved == layerPan)
    {
        //[UserSliderCode_layerPan] -- add your slider handling code here..
        //[/UserSliderCode_layerPan]
    }
    else if (sliderThatWasMoved == layerPitch)
    {
        //[UserSliderCode_layerPitch] -- add your slider handling code here..
        //[/UserSliderCode_layerPitch]
    }
    else if (sliderThatWasMoved == fxSend1)
    {
        //[UserSliderCode_fxSend1] -- add your slider handling code here..
        //[/UserSliderCode_fxSend1]
    }
    else if (sliderThatWasMoved == fxSend2)
    {
        //[UserSliderCode_fxSend2] -- add your slider handling code here..
        //[/UserSliderCode_fxSend2]
    }
    else if (sliderThatWasMoved == fxSend3)
    {
        //[UserSliderCode_fxSend3] -- add your slider handling code here..
        //[/UserSliderCode_fxSend3]
    }
    else if (sliderThatWasMoved == fxSend4)
    {
        //[UserSliderCode_fxSend4] -- add your slider handling code here..
        //[/UserSliderCode_fxSend4]
    }
    else if (sliderThatWasMoved == keyLength)
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
    LayerItem layer (layersListBox->getSelectedLayer());
    KeyItem key (display->getInstrument()->getKey (layer.getNote()));
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == triggerMode)
    {
        //[UserComboBoxCode_triggerMode] -- add your combo box handling code here..
        key.setTriggerMode (triggerMode->getSelectedItemIndex());
        //[/UserComboBoxCode_triggerMode]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode]
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
        display->setNote (midi.getNoteNumber());
        updateControls (dontSendNotification);
    }

    if (interface) interface->handleMidi (midi);
}

void SamplerView::onKeySelected (int k)
{
    display->selectNote (k, true);
    const KeyItem item (display->selectedKey());
    layersListBox->setKeyItem (item);
    updateControls (dontSendNotification);
    if (interface) {
        interface->getKeyForNote (k);
    }
}

void SamplerView::onDisplayUpdate()
{

}

void SamplerView::loadFile (const File& file) {
    if (interface) {
        interface->loadFile (file);
    }
}

void SamplerView::loadProgram (const Programming::Item& item)
{
#if 0
    //FIXME:
    editor.sampler()->currentSynth()->clearKeyboard();
    display->instrument()->load (item.asFile(), display->progressBarSink());
    editor.sampler()->currentSynth()->setInstrument (display->instrument());
    display->setTitle (display->instrument()->getProperty (Slugs::name));
    layersListBox->setKeyItem (display->selectedKey());
#endif
}

void SamplerView::layerChosen()
{
    if (EditScreen* screen = dynamic_cast<EditScreen*> (display->findChildWithID ("edit-screen")))
    {
        LayerItem layer (layersListBox->getSelectedLayer());
        screen->resized();
    }

    updateControls();
}

void SamplerView::updateControls (NotificationType n)
{
    InstrumentPtr instrument (display->getInstrument());
    LayerItem layer (layersListBox->getSelectedLayer());
    KeyItem key (instrument->getKey (layer.getNote()));

    // instrument controls
    volume->getValueObject().referTo (instrument->getPropertyAsValue (Slugs::volume));

    const bool isControllingLayer = this->layerKeyButton->getToggleState();

    if (isControllingLayer)
    {
        layerVolLabel->setText ("Layer Vol.", n);
        layerPitchLabel->setText ("Layer Pitch", n);
        layerPanLabel->setText ("Layer Pan", n);
    }
    else
    {
        layerVolLabel->setText ("Key Vol.", n);
        layerPitchLabel->setText ("Key Pitch", n);
        layerPanLabel->setText ("Key Pan", n);
    }

    // key controls
    if (key.isValid())
    {
        voiceGroup->getValueObject().referTo (key.getPropertyAsValue (Tags::voiceGroup));
        triggerMode->setSelectedItemIndex (key.getTriggerMode(), n);

        if (! isControllingLayer)
        {
            layerVolume->getValueObject().referTo (key.getPropertyAsValue (Slugs::volume));
            layerPan->getValueObject().referTo (key.getPropertyAsValue (Tags::panning));
            layerPitch->getValueObject().referTo (key.getPropertyAsValue (Slugs::pitch));
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

        layerVolume->getValueObject().referTo (layer.getPropertyAsValue (Slugs::volume));
        layerPan->getValueObject().referTo (layer.getPropertyAsValue (Tags::panning));
        layerPitch->getValueObject().referTo (layer.getPropertyAsValue (Slugs::pitch));
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

void SamplerView::setInstrment (InstrumentPtr i)
{
    if (interface)
        interface->setInstrument (i);
    display->setInstrument (i);
    stabilizeView();
}

void SamplerView::setInterface (SamplerInterface *iface)
{
    interface = iface;
    if (interface) {
        display->setInstrument (interface->getInstrument());
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


#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
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
         fontname="Default font" fontsize="11.599999999999999645" bold="0"
         italic="1" justification="33"/>
  <SLIDER name="layerVolume" id="bf65c3b220f44da5" memberName="layerVolume"
          virtualName="" explicitFocusOrder="0" pos="207 178R 56 56" tooltip="Key Volume"
          bkgcol="0" trackcol="7fffffff" rotarysliderfill="73000000" rotaryslideroutline="ff2a2a2a"
          min="-30" max="12" int="0.0010000000000000000208" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="voiceGroup" id="947ac3f3088f71a6" memberName="voiceGroup"
          virtualName="" explicitFocusOrder="0" pos="103 24R 85 13" posRelativeX="5fe3d96c50772121"
          posRelativeY="5fe3d96c50772121" tooltip="Voice Group" bkgcol="0"
          trackcol="7fffffff" rotarysliderfill="ffaeaeae" rotaryslideroutline="ff565656"
          min="-1" max="7" int="1" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="LayersLabel" id="b92c4c02f15c9879" memberName="layersLabel"
         virtualName="" explicitFocusOrder="0" pos="4 152R 172 24" textCol="ffcdcdcd"
         edTextCol="ff000000" edBkgCol="0" labelText="LAYERS" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="1" italic="0" justification="36"/>
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
                    explicitFocusOrder="0" pos="29R 4Rr 21 179" class="DecibelScaleComponent"
                    params=""/>
  <SLIDER name="volume" id="47a6f6acb9d0b4b3" memberName="volume" virtualName=""
          explicitFocusOrder="0" pos="112R 177R 61 58" tooltip="Master Volume"
          bkgcol="0" trackcol="7fffffff" rotarysliderfill="ff5591ad" rotaryslideroutline="ff2a2a2a"
          min="-70" max="6" int="0.0010000000000000000208" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="layer-pan" id="1de5ef00d853a1b0" memberName="layerPan"
          virtualName="" explicitFocusOrder="0" pos="273 162R 56 56" tooltip="Panning"
          bkgcol="0" trackcol="7fffffff" rotarysliderfill="73000000" rotaryslideroutline="ff2a2a2a"
          min="0" max="1" int="0.0010000000000000000208" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="layer:pitch" id="86fb2e7a1567e16" memberName="layerPitch"
          virtualName="" explicitFocusOrder="0" pos="336 178R 56 56" tooltip="Pitch"
          bkgcol="0" trackcol="7fffffff" rotarysliderfill="73000000" rotaryslideroutline="ff2a2a2a"
          min="-24" max="24" int="0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="key:fx-send-1" id="19fcb4fb69e6ec74" memberName="fxSend1"
          virtualName="" explicitFocusOrder="0" pos="283R 154R 40 40" tooltip="FX Send 1"
          bkgcol="0" trackcol="ffeeeeee" rotarysliderfill="73000000" rotaryslideroutline="ff2a2a2a"
          min="0" max="1" int="0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="key:fx-send-2" id="69422f84d84ad59c" memberName="fxSend2"
          virtualName="" explicitFocusOrder="0" pos="244R 176R 40 40" tooltip="FX Send 2"
          bkgcol="ff000000" trackcol="7fffffff" rotarysliderfill="73000000"
          rotaryslideroutline="ff2a2a2a" min="0" max="1" int="0" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <TABBEDCOMPONENT name="mediaTabs" id="8dd080e8355410ba" memberName="mediaTabs"
                   virtualName="" explicitFocusOrder="0" pos="4 4 172 162M" orientation="top"
                   tabBarDepth="25" initialTab="0">
    <TAB name="Asset Library" colour="ffd3d3d3" useJucerComp="0" contentClassName="AssetsListBox"
         constructorParams="" jucerComponentFile=""/>
  </TABBEDCOMPONENT>
  <JUCERCOMP name="articulationControls" id="48885e920b3a8bc2" memberName="articulationControls"
             virtualName="" explicitFocusOrder="0" pos="14 34 85 13" posRelativeX="5fe3d96c50772121"
             posRelativeY="5fe3d96c50772121" sourceFile="ArticulationControls.cpp"
             constructorParams=""/>
  <SLIDER name="key:fx-send-1" id="1fde028392ea9574" memberName="fxSend3"
          virtualName="" explicitFocusOrder="0" pos="204R 155R 40 40" tooltip="FX Send 1"
          bkgcol="0" trackcol="7fffffff" rotarysliderfill="73000000" rotaryslideroutline="ff2a2a2a"
          min="0" max="1" int="0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="key:fxSend4" id="d533f517dda027c3" memberName="fxSend4"
          virtualName="" explicitFocusOrder="0" pos="164R 173R 40 40" tooltip="FX Send 2"
          bkgcol="0" trackcol="7fffffff" rotarysliderfill="73000000" rotaryslideroutline="ff2a2929"
          min="0" max="1" int="0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="layerVolLabel" id="4c9d302bf2827bcb" memberName="layerVolLabel"
         virtualName="" explicitFocusOrder="0" pos="206 128R 58 15" textCol="ffdadada"
         edTextCol="ff000000" edBkgCol="0" hiliteCol="261151ee" labelText="Layer Vol."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="11.599999999999999645" bold="0"
         italic="1" justification="36"/>
  <LABEL name="layerPanLabel" id="aad53eaeda5c6183" memberName="layerPanLabel"
         virtualName="" explicitFocusOrder="0" pos="274 177R 58 15" textCol="ffdadada"
         edTextCol="ff000000" edBkgCol="0" hiliteCol="261151ee" labelText="Layer Pan"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="11.599999999999999645" bold="0"
         italic="1" justification="36"/>
  <LABEL name="layerPitchLabel" id="9bd479de36829fb4" memberName="layerPitchLabel"
         virtualName="" explicitFocusOrder="0" pos="336 126R 58 15" textCol="ffdadada"
         edTextCol="ff000000" edBkgCol="0" hiliteCol="261151ee" labelText="Layer Pitch"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="11.599999999999999645" bold="0"
         italic="1" justification="36"/>
  <LABEL name="fx1Label" id="157d12cf7acd6a0d" memberName="fx1Label" virtualName=""
         explicitFocusOrder="0" pos="278R 170R 31 18" textCol="ffdadada"
         edTextCol="ff000000" edBkgCol="0" hiliteCol="261151ee" labelText="Aux 1"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="11.599999999999999645" bold="0"
         italic="1" justification="33"/>
  <LABEL name="fx1Label" id="672db88d55e40cd9" memberName="fx1Label2"
         virtualName="" explicitFocusOrder="0" pos="238R 144R 31 18" textCol="ffdadada"
         edTextCol="ff000000" edBkgCol="0" hiliteCol="261151ee" labelText="Aux 2"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="11.599999999999999645" bold="0"
         italic="1" justification="33"/>
  <LABEL name="fx1Label" id="278b158d0bb943f9" memberName="fx1Label3"
         virtualName="" explicitFocusOrder="0" pos="200R 170R 31 18" textCol="ffdadada"
         edTextCol="ff000000" edBkgCol="0" hiliteCol="261151ee" labelText="Aux 3"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="11.599999999999999645" bold="0"
         italic="1" justification="33"/>
  <LABEL name="fx1Label" id="71864d4f9ce36d52" memberName="fx1Label4"
         virtualName="" explicitFocusOrder="0" pos="159R 141R 31 18" textCol="ffdadada"
         edTextCol="ff000000" edBkgCol="0" hiliteCol="261151ee" labelText="Aux 4"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="11.599999999999999645" bold="0"
         italic="1" justification="33"/>
  <GENERICCOMPONENT name="keyboard" id="99aac150ed63807" memberName="keyboard" virtualName=""
                    explicitFocusOrder="0" pos="187 4Rr 223M 101" class="KeyboardWidget"
                    params="keyboardState"/>
  <JUCERCOMP name="display" id="56c24b8a829e534f" memberName="display" virtualName=""
             explicitFocusOrder="0" pos="186 27 188M 212M" sourceFile="SamplerDisplay.cpp"
             constructorParams=""/>
  <SLIDER name="keyLength" id="a09d8e14cc07f187" memberName="keyLength"
          virtualName="" explicitFocusOrder="0" pos="-84C 25R 80 13" posRelativeX="5fe3d96c50772121"
          posRelativeY="5fe3d96c50772121" tooltip="Set the keyspan of the selected note(s)"
          min="0" max="127" int="1" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
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


//[EndFile]
//[/EndFile]

}} /* namespace KSP1::Gui */
