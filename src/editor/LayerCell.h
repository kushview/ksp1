/*
    Copyright 2014, Kushview, LLC
    This is an automatically generated GUI class created by the Introjucer!
*/

#ifndef __JUCE_HEADER_D6AE3AF7AC8ED66E__
#define __JUCE_HEADER_D6AE3AF7AC8ED66E__

//[Headers]
#include "KSP1.h"
#include "../Instrument.h"

namespace KSP1 {
namespace Gui {
    class LayersListBox;
}}
//[/Headers]



namespace KSP1 {
namespace Gui {

/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class LayerCell  : public Component
{
public:
    LayerCell ();
    ~LayerCell();

    //[UserMethods]
    void setLayer (const LayerItem& item);
    void setSelected (bool yn);
    int layerIndex() const { return (layer.isValid()) ? layer.index() : -1; }
    void mouseWheelMove (const MouseEvent &e, const MouseWheelDetails &d);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();



private:
    //[UserVariables]
    bool isSelected;
    LayerItem layer;
    ScopedPointer<AudioThumbnail> peak;
    LayersListBox* getListBox();
    String lastResource;
    friend class LayersListBox;
    //[/UserVariables]

    Path internalPath1;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LayerCell)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

}} /* namespace BTV::Gui */

#endif   // __JUCE_HEADER_D6AE3AF7AC8ED66E__
