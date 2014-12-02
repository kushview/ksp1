/*
    Copyright 2013  Kushview, LLC.  All rights reserved.
    This is an automatically generated GUI class created by the Introjucer!
*/

#ifndef __JUCE_HEADER_D8BD5CDE2492A644__
#define __JUCE_HEADER_D8BD5CDE2492A644__

//[Headers]
#include "editor/Screens.h"
//[/Headers]



namespace KSP1 {
namespace Gui {

/**
                                                                    //[Comments]
                                                                    //[/Comments]
*/
class BrowseScreen  : public Screen
{
public:
    BrowseScreen (SamplerDisplay& disp);
    ~BrowseScreen();

    //[UserMethods]
    void onAssetSelected (const AssetItem& item);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();



private:
    //[UserVariables]
    //[/UserVariables]



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BrowseScreen)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

}} /* namespace BTV::Gui */

#endif   // __JUCE_HEADER_D8BD5CDE2492A644__
