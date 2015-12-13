#ifndef KSP1_WORLD_H
#define KSP1_WORLD_H

#include "KSP1.h"
#include "AudioEngine.h"

namespace KSP1 {

class World :  public Element::WorldBase
{
public:
    World();
    ~World();

    DeviceManager& getDevices();
    AudioEnginePtr getEngine();
    PluginManager& getPlugins();

private:
    friend class Application;
    void init();
    class Impl;
    ScopedPointer<Impl> impl;
};

}

#endif // KSP1_WORLD_H
