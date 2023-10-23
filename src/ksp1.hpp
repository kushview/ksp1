
#pragma once

#include "juceconfig.hpp"
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_core/juce_core.h>

namespace ksp1 {

inline static int generateObjectID (int salt = 0) {
    using juce::Random;
    using juce::Range;
    using juce::Time;

    Random r (salt == 0 ? Time::currentTimeMillis() : salt);

    static int lastGenerated = 0;

    int newId = 0;
    while (newId == 0 || newId == lastGenerated)
        newId = r.nextInt (Range<int> (1, std::numeric_limits<int>::max()));

    lastGenerated = newId;
    return newId;
}

} // namespace ksp1
