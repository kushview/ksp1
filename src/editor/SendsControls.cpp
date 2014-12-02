/*
    Copyright 2014, Kushview, LLC
    This is an automatically generated GUI class created by the Introjucer!
*/

//[Headers]
//[/Headers]

#include "SendsControls.h"


namespace KSP1 {
namespace Gui {

//[MiscUserDefs]
namespace KSP1 {
namespace Gui {
//[/MiscUserDefs]

SendsControls::SendsControls ()
{
    addAndMakeVisible (slider = new Slider ("new slider"));
    slider->setRange (0, 10, 0);
    slider->setSliderStyle (Slider::Rotary);
    slider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    slider->addListener (this);

    addAndMakeVisible (slider2 = new Slider ("new slider"));
    slider2->setRange (0, 10, 0);
    slider2->setSliderStyle (Slider::Rotary);
    slider2->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    slider2->addListener (this);

    addAndMakeVisible (label = new Label ("new label",
                                          TRANS("Aux 1")));
    label->setFont (Font (15.00f, Font::plain));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label2 = new Label ("new label",
                                           TRANS("Aux 2")));
    label2->setFont (Font (15.00f, Font::plain));
    label2->setJustificationType (Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (groupComponent = new GroupComponent ("new group",
                                                            TRANS("effects")));
    groupComponent->setTextLabelPosition (Justification::centredLeft);
    groupComponent->setColour (GroupComponent::outlineColourId, Colour (0x66949494));
    groupComponent->setColour (GroupComponent::textColourId, Colour (0xffe3f5ff));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (160, 100);


    //[Constructor]
    //[/Constructor]
}

SendsControls::~SendsControls()
{
    //[Destructor_pre]
    //[/Destructor_pre]

    slider = nullptr;
    slider2 = nullptr;
    label = nullptr;
    label2 = nullptr;
    groupComponent = nullptr;


    //[Destructor]
    //[/Destructor]
}

void SendsControls::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SendsControls::resized()
{
    slider->setBounds (24, 24, 63, 48);
    slider2->setBounds (96, 24, 63, 48);
    label->setBounds (32, 66, 71, 24);
    label2->setBounds (105, 68, 48, 21);
    groupComponent->setBounds (0, 0, proportionOfWidth (0.9794f), proportionOfHeight (0.9801f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SendsControls::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == slider)
    {
        //[UserSliderCode_slider] -- add your slider handling code here..
        //[/UserSliderCode_slider]
    }
    else if (sliderThatWasMoved == slider2)
    {
        //[UserSliderCode_slider2] -- add your slider handling code here..
        //[/UserSliderCode_slider2]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode]
//[/MiscUserCode]


#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SendsControls" template="../../jucer/Templates/GuiTemplate.cpp"
                 componentName="" parentClasses="public Component" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="160" initialHeight="100">
  <BACKGROUND backgroundColour="0"/>
  <SLIDER name="new slider" id="cb91175318bac550" memberName="slider" virtualName=""
          explicitFocusOrder="0" pos="24 24 63 48" min="0" max="10" int="0"
          style="Rotary" textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="d81489b32621e41" memberName="slider2" virtualName=""
          explicitFocusOrder="0" pos="96 24 63 48" min="0" max="10" int="0"
          style="Rotary" textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="ea37ebae5fcd85a7" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="32 66 71 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Aux 1" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="93814ec8dced8c8e" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="105 68 48 21" edTextCol="ff000000"
         edBkgCol="0" labelText="Aux 2" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <GROUPCOMPONENT name="new group" id="5bd462104a374dd9" memberName="groupComponent"
                  virtualName="" explicitFocusOrder="0" pos="0 0 97.959% 97.994%"
                  outlinecol="66949494" textcol="ffe3f5ff" title="effects" textpos="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile]
}}
//[/EndFile]

}} /* namespace KSP1::Gui */
