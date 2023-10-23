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
//[/Headers]

#include "SendsControls.h"

namespace KSP1 {

//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
SendsControls::SendsControls() {
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    slider.reset (new Slider ("new slider"));
    addAndMakeVisible (slider.get());
    slider->setRange (0, 10, 0);
    slider->setSliderStyle (Slider::Rotary);
    slider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    slider->addListener (this);

    slider->setBounds (24, 24, 63, 48);

    slider2.reset (new Slider ("new slider"));
    addAndMakeVisible (slider2.get());
    slider2->setRange (0, 10, 0);
    slider2->setSliderStyle (Slider::Rotary);
    slider2->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    slider2->addListener (this);

    slider2->setBounds (96, 24, 63, 48);

    label.reset (new Label ("new label",
                            TRANS ("Aux 1")));
    addAndMakeVisible (label.get());
    label->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label->setBounds (32, 66, 71, 24);

    label2.reset (new Label ("new label",
                             TRANS ("Aux 2")));
    addAndMakeVisible (label2.get());
    label2->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label2->setJustificationType (Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label2->setBounds (105, 68, 48, 21);

    groupComponent.reset (new GroupComponent ("new group",
                                              TRANS ("effects")));
    addAndMakeVisible (groupComponent.get());
    groupComponent->setTextLabelPosition (Justification::centredLeft);
    groupComponent->setColour (GroupComponent::outlineColourId, Colour (0x66949494));
    groupComponent->setColour (GroupComponent::textColourId, Colour (0xffe3f5ff));

    //[UserPreSize]
    //[/UserPreSize]

    setSize (160, 100);

    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

SendsControls::~SendsControls() {
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    slider         = nullptr;
    slider2        = nullptr;
    label          = nullptr;
    label2         = nullptr;
    groupComponent = nullptr;

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SendsControls::paint (Graphics& g) {
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SendsControls::resized() {
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    groupComponent->setBounds (0, 0, proportionOfWidth (0.9796f), proportionOfHeight (0.9799f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SendsControls::sliderValueChanged (Slider* sliderThatWasMoved) {
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == slider.get()) {
        //[UserSliderCode_slider] -- add your slider handling code here..
        //[/UserSliderCode_slider]
    } else if (sliderThatWasMoved == slider2.get()) {
        //[UserSliderCode_slider2] -- add your slider handling code here..
        //[/UserSliderCode_slider2]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]

//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SendsControls" template="../../jucer/Templates/GuiTemplate.cpp"
                 componentName="" parentClasses="public Component" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="160" initialHeight="100">
  <BACKGROUND backgroundColour="0"/>
  <SLIDER name="new slider" id="cb91175318bac550" memberName="slider" virtualName=""
          explicitFocusOrder="0" pos="24 24 63 48" min="0" max="1e1" int="0"
          style="Rotary" textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="new slider" id="d81489b32621e41" memberName="slider2" virtualName=""
          explicitFocusOrder="0" pos="96 24 63 48" min="0" max="1e1" int="0"
          style="Rotary" textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <LABEL name="new label" id="ea37ebae5fcd85a7" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="32 66 71 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Aux 1" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="1.5e1"
         kerning="0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="93814ec8dced8c8e" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="105 68 48 21" edTextCol="ff000000"
         edBkgCol="0" labelText="Aux 2" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="1.5e1"
         kerning="0" bold="0" italic="0" justification="33"/>
  <GROUPCOMPONENT name="new group" id="5bd462104a374dd9" memberName="groupComponent"
                  virtualName="" explicitFocusOrder="0" pos="0 0 98% 97.976%" outlinecol="66949494"
                  textcol="ffe3f5ff" title="effects" textpos="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

} // namespace KSP1

//[EndFile] You can add extra defines here...
//[/EndFile]
