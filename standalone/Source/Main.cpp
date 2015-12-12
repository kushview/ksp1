
#include "KSP1.h"
#include "AppController.h"
#include "PluginProcessor.h"
#include "URIs.h"
#include "World.h"


#include "MainWindow.h"


namespace KSP1 {

void updateLv2Path()
{
#if JUCE_DEBUG
    const File appFile = File::getSpecialLocation (File::currentApplicationFile);
    String path = appFile.getParentDirectory().getParentDirectory().getChildFile("plugins").getFullPathName();
    path << ":/usr/local/lib/lv2:/usr/lib/lv2:" << getenv("HOME") << "/.lv2";
    setenv ("LV2_PATH", path.toRawUTF8(), 1);
    DBG("LV2_PATH=" << getenv("LV2_PATH"));
#endif
}

class Application  : public JUCEApplication
{
public:
    Application() { }

    const String getApplicationName()    override { return ProjectInfo::projectName; }
    const String getApplicationVersion() override { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed()    override { return false; }

    void initialise (const String& commandLine) override
    {
        updateLv2Path();
        world = new World();


#if 0
        if (LV2PluginFormat* lv2 = world->plugins->format<LV2PluginFormat>())
        {
            const StringArray list = lv2->searchPathsForPlugins(FileSearchPath(), false);
            for (const auto& name : list) {
                Logger::writeToLog (name);
            }

            PluginDescription desc;
            desc.pluginFormatName = "LV2";
            desc.fileOrIdentifier = KSP1_URI;
            auto* plugin = dynamic_cast<Element::Processor*>(
                               lv2->createInstanceFromDescription (desc, 44100.0, 1024));
            if (plugin)
            {
                DBG (plugin->getName() << " loaded ok.");
                if (auto* ui = plugin->createEditorIfNeeded())
                    mainWindow->setContentOwned(ui, false);
                else
                    DBG("Could not open GUI");

                deleteAndZero (plugin);
            }
        }
#endif
        const bool fullScreen = commandLine.contains ("--full-screen");
        mainWindow = new MainWindow ("KSP1 Sampler");

        auto* proc = PluginProcessor::create();
        if (auto* ed = proc->createEditor()) {
            mainWindow->setContentOwned (ed, false);
        }

        if (fullScreen)
        {
            mainWindow->setFullScreen();
        }
        else
        {
            mainWindow->setUsingNativeTitleBar (true);
            mainWindow->setResizable (true, false);
        }
    }

    void shutdown() override
    {
        mainWindow = nullptr;
        world = nullptr;
    }

    void systemRequestedQuit() override
    {
        quit();
    }

    void anotherInstanceStarted (const String& commandLine) override { }

private:
    ScopedPointer<MainWindow> mainWindow;
    ScopedPointer<World> world;
    ScopedPointer<AppController> app;
};
}

START_JUCE_APPLICATION (KSP1::Application)
