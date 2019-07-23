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
//[/Headers]



namespace KSP1 {

//==============================================================================
/**
                                                                    //[Comments]
                                                                    //[/Comments]
*/
class BrowseScreen  : public Screen
{
public:
    //==============================================================================
    BrowseScreen (SamplerDisplay& disp);
    ~BrowseScreen();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void onAssetSelected (const AssetItem& item);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BrowseScreen)
};

} /* KSP1 */

//[EndFile] You can add extra defines here...
//[/EndFile]

