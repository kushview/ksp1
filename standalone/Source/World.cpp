#include "KSP1.h"

#include "AudioEngine.h"
#include "World.h"

namespace KSP1 {
class World::Impl {
public:
    Impl (World& owner) : world (owner)
    {

    }

    void init()
    {
        plugins = new PluginManager();
        plugins->addDefaultFormats();
        devices = new DeviceManager();
        engine  = new AudioEngine (world);
        devices->attach (engine);
    }

    ScopedPointer<DeviceManager> devices;
    AudioEnginePtr engine;
    ScopedPointer<PluginManager> plugins;
    World& world;
};

World::World() { }
World::~World()
{
    impl = nullptr;
}

void World::init()
{
    impl = new Impl (*this);
    impl->init();
}

DeviceManager& World::getDevices() { return *impl->devices; }
AudioEnginePtr World::getEngine()  { return impl->engine; }
PluginManager& World::getPlugins() { return *impl->plugins; }

}
