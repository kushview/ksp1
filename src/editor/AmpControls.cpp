/*
    Copyright 2014 Kushview, LLC.  All rights reserved
    This is an automatically generated GUI class created by the Introjucer!
*/

//[Headers]
//[/Headers]

#include "AmpControls.h"


namespace KSP1 {
namespace Gui {

//[MiscUserDefs]
//[/MiscUserDefs]

AmpControls::AmpControls ()
{
    addAndMakeVisible (slider = new Slider ("new slider"));
    slider->setRange (0, 10, 0);
    slider->setSliderStyle (Slider::LinearVertical);
    slider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    slider->addListener (this);

    addAndMakeVisible (slider2 = new Slider ("new slider"));
    slider2->setRange (0, 10, 0);
    slider2->setSliderStyle (Slider::LinearVertical);
    slider2->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    slider2->addListener (this);

    addAndMakeVisible (slider3 = new Slider ("new slider"));
    slider3->setRange (0, 10, 0);
    slider3->setSliderStyle (Slider::LinearVertical);
    slider3->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    slider3->addListener (this);

    addAndMakeVisible (slider4 = new Slider ("new slider"));
    slider4->setRange (0, 10, 0);
    slider4->setSliderStyle (Slider::LinearVertical);
    slider4->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    slider4->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (200, 100);


    //[Constructor]
    //[/Constructor]
}

AmpControls::~AmpControls()
{
    //[Destructor_pre]
    //[/Destructor_pre]

    slider = nullptr;
    slider2 = nullptr;
    slider3 = nullptr;
    slider4 = nullptr;


    //[Destructor]
    //[/Destructor]
}

void AmpControls::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xad615b5b));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void AmpControls::resized()
{
    slider->setBounds (8, 8, 24, 80);
    slider2->setBounds (32, 8, 24, 80);
    slider3->setBounds (56, 8, 24, 80);
    slider4->setBounds (80, 8, 24, 80);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void AmpControls::sliderValueChanged (Slider* sliderThatWasMoved)
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
    else if (sliderThatWasMoved == slider3)
    {
        //[UserSliderCode_slider3] -- add your slider handling code here..
        //[/UserSliderCode_slider3]
    }
    else if (sliderThatWasMoved == slider4)
    {
        //[UserSliderCode_slider4] -- add your slider handling code here..
        //[/UserSliderCode_slider4]
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

<JUCER_COMPONENT documentType="Component" className="AmpControls" template="../../jucer/Templates/GuiTemplate.cpp"
                 componentName="" parentClasses="public Component" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="200" initialHeight="100">
  <BACKGROUND backgroundColour="ad615b5b"/>
  <SLIDER name="new slider" id="aaaed040846e1d80" memberName="slider" virtualName=""
          explicitFocusOrder="0" pos="8 8 24 80" min="0" max="10" int="0"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="fe8eb455e21f6840" memberName="slider2"
          virtualName="" explicitFocusOrder="0" pos="32 8 24 80" min="0"
          max="10" int="0" style="LinearVertical" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="364ba017adeefc7d" memberName="slider3"
          virtualName="" explicitFocusOrder="0" pos="56 8 24 80" min="0"
          max="10" int="0" style="LinearVertical" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="7e87eb39dbfac003" memberName="slider4"
          virtualName="" explicitFocusOrder="0" pos="80 8 24 80" min="0"
          max="10" int="0" style="LinearVertical" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile]
//[/EndFile]

}} /* namespace KSP1::Gui */
