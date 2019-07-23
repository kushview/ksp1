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

#include "editor/Screens.h"

#include "Instrument.h"
#include "KSP1.h"
#include "Controller.h"

namespace KSP1 {
    class SamplerInterface;
namespace Gui {
    class ScreenManager;
    class DisplayProgressBar;
}}

//[/Headers]



namespace KSP1 {

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class SamplerDisplay  : public Component,
                        public Timer,
                        public ComboBox::Listener,
                        public Label::Listener
{
public:
    //==============================================================================
    SamplerDisplay ();
    ~SamplerDisplay();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    typedef boost::signals2::connection Connection;

    KeyItem selectedKey();
    LayerItem selectedLayer();
    inline int32 selectedNote() const { return currentNote; }
    void selectNote (int32 note, bool notify = true);

    void setNote (int32 n);
    void setTitle (const String& titleText);

    void setScreen (Screen::ID);

    //SamplerInterface* getInterface() const;
    /** Get/set the current instrument in current pattern */
    void setInstrument (InstrumentPtr i);
    InstrumentPtr getInstrument() const;

    template<class ClientType> inline
    Connection connectUpdateClient (ClientType& client) {
        return updateSignal.connect (boost::bind (&ClientType::onDisplayUpdate, &client));
    }

    ProgressSink& progressBarSink();

    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void labelTextChanged (Label* labelThatHasChanged) override;
    void mouseDown (const MouseEvent& e) override;
    void mouseDoubleClick (const MouseEvent& e) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    uint32 currentLayer;
    int currentNote;
    Element::Signal updateSignal;

    class Dispatch;
    friend class Dispatch;
    ScopedPointer<Dispatch> dispatch;

    void timerCallback();
    void runDispatchLoop();

    class Models;
    friend class Models;
    ScopedPointer<Models> models;

    ScopedPointer<ScreenManager> screens;
    Screen* currentScreen;
    friend class ContentComponent;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<ComboBox> comboBox;
    std::unique_ptr<Label> title;
    std::unique_ptr<Label> noteLabel;
    std::unique_ptr<Component> screen;
    std::unique_ptr<DisplayProgressBar> progressBar;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SamplerDisplay)
};

} /* KSP1 */

//[EndFile] You can add extra defines here...
//[/EndFile]

