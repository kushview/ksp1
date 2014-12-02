/*
    Copyright 2014, Kushview, LLC
    This is an automatically generated GUI class created by the Introjucer!
*/

#ifndef __JUCE_HEADER_7FF0250E2BF56D64__
#define __JUCE_HEADER_7FF0250E2BF56D64__

//[Headers]
#include "JuceHeader.h"
//[/Headers]



namespace KSP1 {
namespace Gui {

/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class FilterControls  : public Component,
                        public ComboBoxListener
{
public:
    FilterControls ();
    ~FilterControls();

    //[UserMethods]
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);



private:
    //[UserVariables]
    //[/UserVariables]

    ScopedPointer<ComboBox> filterTypes;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterControls)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

}} /* namespace BTV::Gui */

#endif   // __JUCE_HEADER_7FF0250E2BF56D64__
