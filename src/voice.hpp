/*
    This file is part of KSP1
    Copyright (C) 2014  Kushview, LLC. All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#pragma once

#include <cstdint>

#include "adsr.hpp"
#include "juceconfig.hpp"
#include <juce_audio_basics/juce_audio_basics.h>

namespace ksp1 {

class SamplerSound;
class SamplerSynth;

class SamplerVoice : public juce::SynthesiserVoice {
public:
    enum State {
        StopRequested,
        Stopped,
        PlayRequested,
        Playing,
        Releasing
    };

    SamplerVoice (SamplerSynth& parent, int voiceId);
    ~SamplerVoice();

    bool canPlaySound (juce::SynthesiserSound* sound);
    void startNote (const int midiNote,
                    const float velocity,
                    juce::SynthesiserSound* sound,
                    const int pitchPosition);
    void pitchWheelMoved (const int newValue);
    void controllerMoved (const int controllerNumber, const int newValue);
    void renderNextBlock (juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples);
    void stopNote (float velocity, bool allowTailOff);

protected:
    void setPlaybackState (State s);

private:
    float velocityNormal; // MIDI velocity ranging from  0.0 - 1.0 (linear to 0-127)
    float velocityGain;   // velocityNormal converted to gain
    float wheelPitch;     // Mixin pitch value from pitch messages

    int note;
    int64_t frame;

    State state;
    SamplerSound* sound;
    SamplerSynth& owner;

    int id;
    float lastGains[16];
    double layerPosition[32];

    double pitchRatio;

    juce::AudioSampleBuffer tempBuffer;
    friend class SamplerSynth;

    ADSR adsr;

    void _renderNextBlock (juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples);
};

} // namespace ksp1
