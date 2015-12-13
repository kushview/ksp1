#include "KSP1.h"

#ifndef KSP1_MAIN_WINDOW_H
#define KSP1_MAIN_WINDOW_H

class MainWindow : public DocumentWindow
{
public:
    MainWindow (String name)  : DocumentWindow (name, Colours::lightgrey, 0)
    {
        auto* c = getConstrainer();
        c->setMaximumSize (800, 480);
        c->setMinimumSize (300, 200);
        setResizeLimits (300, 200, 800, 480);

        centreWithSize (480, 320);
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
