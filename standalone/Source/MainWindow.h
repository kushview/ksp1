#include "KSP1.h"

#ifndef KSP1_MAIN_WINDOW_H
#define KSP1_MAIN_WINDOW_H

class MainWindow : public DocumentWindow
{
public:
    MainWindow (String name)  : DocumentWindow (name, Colours::lightgrey, 0)
    {
        centreWithSize (300, 200);
        setVisible (true);
    }

    void closeButtonPressed() override
    {
        JUCEApplication::getInstance()->systemRequestedQuit();
    }

    void setFullScreen()
    {
      getPeer()->setFullScreen (true);
      Desktop::getInstance().setKioskModeComponent (this, false);
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
};

#endif
