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

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "KSP1.h"
#include "../Instrument.h"

namespace KSP1 {
class LayersListBox;
}
//[/Headers]



namespace KSP1 {

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class LayerCell  : public Component
{
public:
    //==============================================================================
    LayerCell ();
    ~LayerCell();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void setLayer (const LayerItem& item);
    void setSelected (bool yn);
    int layerIndex() const { return (layer.isValid()) ? layer.index() : -1; }
    void mouseWheelMove (const MouseEvent &e, const MouseWheelDetails &d) override;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    bool isSelected;
    LayerItem layer;
    ScopedPointer<AudioThumbnail> peak;
    LayersListBox* getListBox();
    String lastResource;
    friend class LayersListBox;
    //[/UserVariables]

    //==============================================================================
    Path internalPath1;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LayerCell)
};

} /* KSP1 */

//[EndFile] You can add extra defines here...
//[/EndFile]

