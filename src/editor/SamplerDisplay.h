/*
    This file is part of KSP1
    Copyright (C) 2014  Kushview, LLC. All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    GUI class created by the Introjucer!
*/

#ifndef __JUCE_HEADER_3B19452B55A2A434__
#define __JUCE_HEADER_3B19452B55A2A434__

//[Headers]

#include "editor/Screens.h"

#include "Instrument.h"
#include "KSP1.h"
#include "Controller.h"

namespace KSP1 {
    class SamplerInterface;
namespace Gui {
    class DisplayProgressBar;
}}

//[/Headers]



namespace KSP1 {
namespace Gui {

/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class SamplerDisplay  : public Component,
                        public Timer,
                        public ComboBoxListener,
                        public LabelListener
{
public:
    SamplerDisplay ();
    ~SamplerDisplay();

    //[UserMethods]

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

    void paint (Graphics& g);
    void resized();
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
    void labelTextChanged (Label* labelThatHasChanged);
    void mouseDown (const MouseEvent& e);
    void mouseDoubleClick (const MouseEvent& e);



private:
    //[UserVariables]
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

    friend class ContentComponent;
    //[/UserVariables]

    ScopedPointer<ComboBox> comboBox;
    ScopedPointer<Label> title;
    ScopedPointer<Label> noteLabel;
    ScopedPointer<Component> screen;
    ScopedPointer<DisplayProgressBar> progressBar;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SamplerDisplay)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

}} /* namespace KSP1::Gui */

#endif   // __JUCE_HEADER_3B19452B55A2A434__
