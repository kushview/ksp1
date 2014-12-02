/*
    Copyright 2013  Kushview, LLC.  All rights reserved.
    This is an automatically generated GUI class created by the Introjucer!
*/

#ifndef __JUCE_HEADER_EBED664241004FFC__
#define __JUCE_HEADER_EBED664241004FFC__

//[Headers]
#include "editor/PanelButton.h"

namespace KSP1 {
    class Globals;
namespace Gui {
    class SamplerDisplay;
    class SamplerEditor;
    class Workspace;
}}

//[/Headers]



namespace KSP1 {
namespace Gui {

/**
                                                                    //[Comments]
                                                                    //[/Comments]
*/
class SamplerPluginView  : public Component,
                           public ButtonListener
{
public:
    SamplerPluginView (SamplerEditor& ed);
    ~SamplerPluginView();

    //[UserMethods]
    void switchPanel (Panel::ID panel);
    SamplerDisplay* display();
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);



private:
    //[UserVariables]
    Globals& world;
    SamplerEditor& owner;
    //Element::Gui::Style style;
    ComponentBoundsConstrainer constrain;
    //[/UserVariables]

    ScopedPointer<Workspace> workspace;
    ScopedPointer<Label> drumbotLabel;
    ScopedPointer<TextButton> textButton;
    ScopedPointer<TextButton> textButton2;
    ScopedPointer<TextButton> textButton3;
    ScopedPointer<TextButton> textButton4;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SamplerPluginView)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

}} /* namespace KSP1::Gui */

#endif   // __JUCE_HEADER_EBED664241004FFC__
