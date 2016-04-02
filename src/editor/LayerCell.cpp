/*
    Copyright 2014, Kushview, LLC
    This is an automatically generated GUI class created by the Introjucer!
*/

//[Headers]
#include "LayersListBox.h"
//[/Headers]

#include "LayerCell.h"


namespace KSP1 {
namespace Gui {

//[MiscUserDefs]
//[/MiscUserDefs]

LayerCell::LayerCell ()
{

    //[UserPreSize]
    //[/UserPreSize]

    setSize (500, 300);


    //[Constructor]
    //[/Constructor]
}

LayerCell::~LayerCell()
{
    //[Destructor_pre]
    //[/Destructor_pre]



    //[Destructor]
    //[/Destructor]
}

void LayerCell::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff7d8272));

    g.setColour (Colours::white);
    g.fillPath (internalPath1);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void LayerCell::resized()
{
    internalPath1.clear();
    internalPath1.startNewSubPath (784.0f, 0.0f);
    internalPath1.lineTo (static_cast<float> (-8), 0.0f);
    internalPath1.closeSubPath();

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode]
void LayerCell::setLayer (const LayerItem& item)
{
    layer = item;

    if (layer.isValid() && ! layer.isEmpty())
    {
        String nextResource = layer.fileString();
        if (! peak || nextResource != lastResource)
        {
            //FIXME: peak = box->data.createPeak (File (nextResource));
            lastResource = nextResource;
        }
    }
}

LayersListBox* LayerCell::getListBox()
{
    LayersListBox* box = findParentComponentOfClass<LayersListBox>();
    assert (box);
    return box;
}

void LayerCell::setSelected (bool yn)
{
    const bool changed = (yn != isSelected);
    isSelected = yn;

    if (isSelected && changed) {
        //std::clog << "layer selected: " << layer.index() << std::endl;
    } else if (changed) {
        //std::clog << "layer unselected " << layer.index() << std::endl;
    }

    if (changed)
        repaint();
}

void LayerCell::mouseWheelMove (const MouseEvent&, const MouseWheelDetails &d)
{
    if (LayersListBox* box = getListBox())
    {
        std::clog << "layer mouse\n";
        if (d.deltaY < 0)
            box->keyPressed (KeyPress (KeyPress::pageDownKey));
        else
            box->keyPressed (KeyPress (KeyPress::pageUpKey));
    }
}
//[/MiscUserCode]


#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
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


//[EndFile]
//[/EndFile]

}} /* namespace KSP1::Gui */
