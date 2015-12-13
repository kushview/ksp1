#include "AudioEngine.h"
#include "World.h"
#include "URIs.h"

namespace KSP1 {

class EngineCallback : public AudioProcessorPlayer
{
public:
    EngineCallback() { }
    ~EngineCallback() { }
};

class AudioEngine::Impl
{
public:
    Impl (World& w) : world(w)
    {
        if (auto* proc = createSampler())
            callback.setProcessor (proc);
    }

    ~Impl() { }

    EngineCallback callback;
    World& world;

    AudioProcessor* createSampler()
    {
        PluginManager& plugins (world.getPlugins());
        String error;
        PluginDescription desc;
        desc.pluginFormatName = "LV2";
        desc.fileOrIdentifier = KSP1_URI;

        auto* plug = plugins.createPlugin (desc, error);

        if (error.isNotEmpty()) {
            DBG("Error loading plugin: " << error);
        }

        return plug;
    }

    AudioProcessorEditor* getCurrentEditor()
    {
        if (auto* proc = callback.getCurrentProcessor())
        {
            if (auto* ed = proc->createEditorIfNeeded())
                return ed;
            else
                return new GenericAudioProcessorEditor (proc);
        }

        return nullptr;
    }
};

AudioEngine::AudioEngine (World& w)
{
    impl = new Impl (w);
}

AudioEngine::~AudioEngine() { }

AudioIODeviceCallback& AudioEngine::callback() { return impl->callback; }
MidiInputCallback& AudioEngine::getMidiInputCallback() { return impl->callback; }
AudioProcessorEditor* AudioEngine::getPluginEditor() {
    return impl->getCurrentEditor();
}

}
