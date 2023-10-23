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
#include "LayersListBox.h"
//[/Headers]

#include "LayerCell.h"

namespace KSP1 {

//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
LayerCell::LayerCell() {
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    internalPath1.startNewSubPath (784.0f, 0.0f);
    internalPath1.lineTo (static_cast<float> (-8), 0.0f);
    internalPath1.closeSubPath();

    //[UserPreSize]
    //[/UserPreSize]

    setSize (500, 300);

    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

LayerCell::~LayerCell() {
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void LayerCell::paint (Graphics& g) {
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff7d8272));

    {
        float x = 0, y = 0;
        Colour fillColour = Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillPath (internalPath1, AffineTransform::translation (x, y));
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void LayerCell::resized() {
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void LayerCell::setLayer (const LayerItem& item) {
    layer = item;

    if (layer.isValid() && ! layer.isEmpty()) {
        String nextResource = layer.fileString();
        if (! peak || nextResource != lastResource) {
            //FIXME: peak = box->data.createPeak (File (nextResource));
            lastResource = nextResource;
        }
    }
}

LayersListBox* LayerCell::getListBox() {
    LayersListBox* box = findParentComponentOfClass<LayersListBox>();
    assert (box);
    return box;
}

void LayerCell::setSelected (bool yn) {
    const bool changed = (yn != isSelected);
    isSelected         = yn;

    if (isSelected && changed) {
        //std::clog << "layer selected: " << layer.index() << std::endl;
    } else if (changed) {
        //std::clog << "layer unselected " << layer.index() << std::endl;
    }

    if (changed)
        repaint();
}

void LayerCell::mouseWheelMove (const MouseEvent&, const MouseWheelDetails& d) {
    if (LayersListBox* box = getListBox()) {
        std::clog << "layer mouse\n";
        if (d.deltaY < 0)
            box->keyPressed (KeyPress (KeyPress::pageDownKey));
        else
            box->keyPressed (KeyPress (KeyPress::pageUpKey));
    }
}
//[/MiscUserCode]

//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="LayerCell" template="../../jucer/Templates/GuiTemplate.cpp"
                 componentName="" parentClasses="public Component" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="500" initialHeight="300">
  <BACKGROUND backgroundColour="ff7d8272">
    <PATH pos="0 0 100 100" fill="solid: ffffffff" hasStroke="0" nonZeroWinding="1">s 784 0 l -8 0 x</PATH>
  </BACKGROUND>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

} // namespace KSP1

//[EndFile] You can add extra defines here...
//[/EndFile]
