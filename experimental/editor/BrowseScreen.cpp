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

//[Headers] You can add your own extra header files here...
#include "editor/AssetsListBox.h"
#include "editor/SamplerDisplay.h"
#include "editor/Screens.h"
#include "Locations.h"
//[/Headers]

#include "BrowseScreen.h"


namespace KSP1 {

//[MiscUserDefs] You can add your own user definitions and misc code here...

class AssetsListBoxHeader : public Component
{
public:

    AssetsListBoxHeader (const String& title = String::empty)
    {
        addAndMakeVisible (assetLabel = new Label ("assetLabel", title));
        setSize (100, 24);
    }

    void resized() {
        assetLabel->setBounds (getLocalBounds());
    }

    ScopedPointer<Label> assetLabel;
};

void BrowseScreen::onAssetSelected (const AssetItem& item)
{
    DBG ("On asset selected (browse screen)");
}

//[/MiscUserDefs]

//==============================================================================
BrowseScreen::BrowseScreen (SamplerDisplay& disp)
    : Screen (disp, "Browse Screen", Screen::browseScreen)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..

    AssetsListBox* list = new AssetsListBox();
    addPage ("Browser", list);
    list->signalSelected.connect (boost::bind (&BrowseScreen::onAssetSelected, this, ::_1));

#if 0
    //FIXME:
    if (const Filesystem* fs = disp.globals().locations().getFilesystems().get(0))
    {
        // list->setRootItem (fs->root());
        // list->setHeaderComponent (new AssetsListBoxHeader (fs->getName()));
    }
#endif


    //[/Constructor]
}

BrowseScreen::~BrowseScreen()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void BrowseScreen::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void BrowseScreen::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="BrowseScreen" template="../../jucer/Templates/GuiTemplate.cpp"
                 componentName="" parentClasses="public Screen" constructorParams="SamplerDisplay&amp; disp"
                 variableInitialisers="Screen (disp, &quot;Browse Screen&quot;, Screen::browseScreen)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ffffff"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif



} /* KSP1 */

//[EndFile] You can add extra defines here...
//[/EndFile]

