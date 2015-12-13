#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include "KSP1.h"

namespace KSP1 {
class World;
class AudioEngine :  public Element::Engine
{
public:
    AudioEngine (World&);
    ~AudioEngine();

    AudioIODeviceCallback& callback() override;
    MidiInputCallback& getMidiInputCallback() override;
    AudioProcessorEditor* getPluginEditor();

private:
    class Impl;
    ScopedPointer<Impl> impl;
};

typedef ReferenceCountedObjectPtr<AudioEngine> AudioEnginePtr;
}

#endif // AUDIOENGINE_H
