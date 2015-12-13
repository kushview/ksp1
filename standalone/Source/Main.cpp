
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
    const File appFile = File::getSpecialLocation (File::invokedExecutableFile);
    String path = appFile.getParentDirectory().getParentDirectory().getChildFile("plugins").getFullPathName();

    DBG("before path:" << appFile.getFullPathName());

    path << ":/usr/local/lib/lv2:/usr/lib/lv2:" << getenv("HOME") << "/.lv2";
    setenv ("LV2_PATH", path.toRawUTF8(), 1);
    DBG("LV2_PATH=" << getenv("LV2_PATH"));
#endif
}

struct CommandLine {
    const bool fullScreen;

    CommandLine(const String& str)
       : fullScreen (str.contains("--full-screen"))
    { }
};

class Application  : public JUCEApplication
{
public:
    Application() { }

    const String getApplicationName()    override { return ProjectInfo::projectName; }
    const String getApplicationVersion() override { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed()    override { return false; }

    void initialise (const String& commandLine) override
    {
        CommandLine cli (commandLine);

        updateLv2Path();
        world = new World();
        world->init();
        app = new AppController (*world);
        ApplicationCommandManager& commands (app->getCommandManager());
        commands.registerAllCommandsForTarget (app);

        bool launched = false;
        if (launchEngine (cli))
            launched = launchGui (cli);
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

    bool launchEngine (CommandLine cli)
    {
        DeviceManager& devices (world->getDevices());
        const String err (devices.initialiseWithDefaultDevices (2, 2));
        return err.isEmpty();
    }

    bool launchGui (CommandLine cli)
    {
        mainWindow = new MainWindow ("KSP1 Sampler");
        mainWindow->setUsingNativeTitleBar (true);
        mainWindow->addKeyListener (app->getCommandManager().getKeyMappings());
        mainWindow->setResizable (true, true);

        if (AudioEnginePtr engine = world->getEngine())
            if (auto* ed = engine->getPluginEditor())
                mainWindow->setContentOwned (ed, true);

        if (cli.fullScreen)
            mainWindow->setFullScreen();

        return true;
    }

private:
    ScopedPointer<MainWindow> mainWindow;
    ScopedPointer<World> world;
    ScopedPointer<AppController> app;
};
}

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

START_JUCE_APPLICATION (KSP1::Application)
