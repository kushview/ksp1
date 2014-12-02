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

#ifndef __JUCE_HEADER_DA15C67923F9927D__
#define __JUCE_HEADER_DA15C67923F9927D__

//[Headers]
#include "KSP1.h"
#include "Banks.h"
#include "Panels.h"
#include "Controller.h"

namespace KSP1 {
namespace Gui {

    class ProgramsListBox;

}}

//[/Headers]



namespace KSP1 {
namespace Gui {

/**
                                                                    //[Comments]
                                                                    //[/Comments]
*/
class ProgramEditorView  : public KSP1::Gui::Panel
{
public:
    ProgramEditorView (Controller& ctrl);
    ~ProgramEditorView();

    //[UserMethods]
    int32 type() const { return KSP1::Gui::Panel::programsPanel; }
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();



private:
    //[UserVariables]
    Controller& controller;
    //[/UserVariables]

    ScopedPointer<KSP1::Gui::BankList> bankList1;
    ScopedPointer<ProgramsListBox> programList;
    ScopedPointer<KSP1::Gui::BankList> banklist2;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProgramEditorView)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

}} /* namespace KSP1::Gui */

#endif   // __JUCE_HEADER_DA15C67923F9927D__
