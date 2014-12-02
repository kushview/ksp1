/*
    Copyright 2014 Kushview, LLC.  All rights reserved
    This is an automatically generated GUI class created by the Introjucer!
*/

//[Headers]
#include "editor/AssetsListBox.h"
#include "editor/SamplerDisplay.h"
#include "editor/Screens.h"
#include "Locations.h"
//[/Headers]

#include "BrowseScreen.h"


namespace KSP1 {
namespace Gui {

//[MiscUserDefs]

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

BrowseScreen::BrowseScreen (SamplerDisplay& disp)
    : Screen (disp, "Browse Screen", Screen::browseScreen)
{

    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor]

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
    //[Destructor_pre]
    //[/Destructor_pre]



    //[Destructor]
    //[/Destructor]
}

void BrowseScreen::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    Screen::paint (g);
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void BrowseScreen::resized()
{
    //[UserResized] Add your own custom resize handling here..
    Screen::resized();
    //[/UserResized]
}



//[MiscUserCode]
//[/MiscUserCode]


#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
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


//[EndFile]
//[/EndFile]

}} /* namespace KSP1::Gui */
