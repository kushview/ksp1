/*
    Copyright 2014, Kushview, LLC
    This is an automatically generated GUI class created by the Introjucer!
*/

#ifndef __JUCE_HEADER_FFBEFC10609F80A2__
#define __JUCE_HEADER_FFBEFC10609F80A2__

//[Headers]
#include "KSP1.h"

namespace KSP1 {

    class Instrument;
    class Programming;

namespace Gui {
    class Screen;
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
class QuickBrowser  : public Component,
                      public ButtonListener
{
public:
    QuickBrowser (Programming& progs);
    ~QuickBrowser();

    //[UserMethods]
    QuickBrowser (Programming& progs, Instrument& i);
    QuickBrowser (Screen& screen);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);



private:
    //[UserVariables]
    Programming& programs;
    InstrumentPtr inst;
    void loadItem (const Programming::Item& prog);
    friend class ProgramsListBox;
    //[/UserVariables]

    ScopedPointer<ProgramsListBox> listBox;
    ScopedPointer<TextButton> textButton;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (QuickBrowser)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

}} /* namespace BTV::Gui */

#endif   // __JUCE_HEADER_FFBEFC10609F80A2__
