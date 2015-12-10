
#include "KSP1.h"

namespace KSP1 {
  class Application  : public JUCEApplication
  {
  public:
      Application() {}

      const String getApplicationName()    override { return ProjectInfo::projectName; }
      const String getApplicationVersion() override { return ProjectInfo::versionString; }
      bool moreThanOneInstanceAllowed()    override { return false; }

      void initialise (const String& commandLine) override
      {
          mainWindow = new MainWindow (getApplicationName());
          mainWindow->getPeer()->setFullScreen (true);
          Desktop::getInstance().setKioskModeComponent (mainWindow, false);
      }

      void shutdown() override
      {
          mainWindow = nullptr;
      }

      void systemRequestedQuit() override
      {
          quit();
      }

      void anotherInstanceStarted (const String& commandLine) override { }

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

      private:
          JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
      };

  private:
      ScopedPointer<MainWindow> mainWindow;
  };
}

START_JUCE_APPLICATION (KSP1::Application)
