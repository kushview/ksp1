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
#include "Banks.h"
#include "Controller.h"
#include "KSP1.h"
#include "Panels.h"

namespace KSP1 {
namespace Gui {

class ProgramsListBox;

}
} // namespace KSP1

//[/Headers]

namespace KSP1 {

//==============================================================================
/**
                                                                    //[Comments]
                                                                    //[/Comments]
*/
class ProgramEditorView : public KSP1::Gui::Panel {
public:
    //==============================================================================
    ProgramEditorView (Controller& ctrl);
    ~ProgramEditorView();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    int32 type() const { return KSP1::Gui::Panel::programsPanel; }
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    Controller& controller;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<KSP1::Gui::BankList> bankList1;
    std::unique_ptr<ProgramsListBox> programList;
    std::unique_ptr<KSP1::Gui::BankList> banklist2;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProgramEditorView)
};

} // namespace KSP1

//[EndFile] You can add extra defines here...
//[/EndFile]
