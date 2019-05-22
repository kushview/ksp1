/*
    This file is part of KSP1
    Copyright (C) 2014-2019  Kushview, LLC. All rights reserved.

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

#include "engine/LayerData.h"
#include "engine/SamplerSounds.h"
#include "engine/SamplerSynth.h"
#include "SamplerVoice.h"

#define KSP1_USE_ADSR             0
#define KSP1_USE_PANNING          1
#define KSP1_USE_VELOCITY_GAIN    1
#define KSP1_VELOCITY_GAIN_RANGE  9.0f
#define KSP1_PITCH_WHEEL_RANGE    12.0f
#define KSP1_USE_VELOCITY_RANGE   0
#define KSP1_USE_VOICE_GROUPS     1

namespace KSP1 {

// TODO: implement ADSR: static const int32 adsrChannel = 0;

SamplerVoice::SamplerVoice (SamplerSynth& parent, int32 voiceId)
    : owner (parent), tempBuffer (1, 1)
{
    id = voiceId;
    sound = nullptr;
    note  = -1;
    frame = 0;
    state = Stopped;
    wheelPitch = 0.f;

    for (int i = 0 ; i < 3; ++i)
        lastGains [i] = 0.0f;

    for (int i = 0; i < 32; ++i)
        layerPosition [i] = 0.0f;
}

SamplerVoice::~SamplerVoice() { }

bool SamplerVoice::canPlaySound (SynthesiserSound* sound) {
    return dynamic_cast<KSP1::SamplerSound*> (sound) != nullptr;
}

void SamplerVoice::pitchWheelMoved (const int newValue)
{
    wheelPitch = (newValue - 8192) * (12.0f / 8192.0f);
}

void SamplerVoice::controllerMoved (const int controllerNumber,  const int newValue)
{
}

void SamplerVoice::startNote (const int midiNote, const float velocity,
                              SynthesiserSound* s,  const int pitchPos)
{
    note     = midiNote;
    sound    = static_cast<KSP1::SamplerSound*> (s);
    frame    = sound->getStart();

    velocityNormal = velocity;
    velocityGain   = Decibels::decibelsToGain (-KSP1_VELOCITY_GAIN_RANGE * (1.0f - velocity),
                                               -KSP1_VELOCITY_GAIN_RANGE - 1);
    wheelPitch     = (pitchPos - 8192) * (KSP1_PITCH_WHEEL_RANGE / 8192.0f);

    jassert (sound != nullptr);

    const KeyInfo& key (sound->getKeyInfo());

    // Handle things that need to alter other voices in one shot
    if (key.triggerMode == TriggerMode::Retrigger || key.voiceGroup >= 0)
    {
        for (int v = owner.getNumVoices(); --v >= 0;)
        {
            KSP1::SamplerVoice *voice = static_cast<KSP1::SamplerVoice*> (owner.getVoice(v));

            if (voice == this)
                continue;

            if (key.triggerMode == TriggerMode::Retrigger && note == voice->getCurrentlyPlayingNote())
            {
                voice->stopNote (0.0f, false);
            }
           #if KSP1_USE_VOICE_GROUPS
            if (voice->sound && key.voiceGroup >= 0 &&
                key.voiceGroup == voice->sound->getKeyInfo().voiceGroup)
            {
                voice->stopNote (0.0f, false);
            }
           #endif
        }
    }

    for (int32 i = sound->getNumLayers(); --i >= 0;) {
        // sound->getLayer(i)->startNote (this->id, key);
        layerPosition [i] = static_cast<double> (sound->getLayer(i)->in.get() + sound->getLayer(i)->getOffset());
    }

   #if KSP1_USE_ADSR
    adsr.sync (key.adsr, true);
    adsr.setFutureRelease ((float) sound->length() - adsr.release());
   #endif

    setPlaybackState (PlayRequested);
}

void SamplerVoice::renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
    if (sound == nullptr)
        return;

    bool isLastCycle = false;

    int endFrame = frame + numSamples;
    if (endFrame >= sound->length())
    {
        numSamples = endFrame - sound->length();
        endFrame   = frame + numSamples;
        isLastCycle = true;
    }

    if (! sound->acquire())
    {
        clearCurrentNote(); 
        sound = nullptr;
        jassertfalse;
        return;
    }

    const KeyInfo& key (sound->getKeyInfo());
    const int32 numLayers = sound->getNumLayers();

    float* outL = outputBuffer.getWritePointer (0, startSample);
    float* outR = outputBuffer.getNumChannels() > 1
                ? outputBuffer.getWritePointer (1, startSample) : nullptr;

   #if KSP1_USE_ADSR
    tempBuffer.setSize (1, numSamples, false, false, true);
    float* adsrGain = tempBuffer.getSampleData (adsrChannel, 0);
    adsr.processBuffer (numSamples, adsrGain);
   #endif

    const float keyPitch = static_cast<float> (key.pitch) + sound->getPitchOffsetForNote (this->note);

    int32 renderFrame = 0;
    for (int32 i = 0; i < numLayers; ++i)
    {
        LayerData* layer = sound->getLayer (i);
        if (endFrame < layer->getStart())
            continue;

        const float* const inL = layer->getSampleData (0, 0);
        const float* const inR = layer->numChannels > 1  ? layer->getSampleData (1, 0) : nullptr;
        const double pitchRatio = pow (2.0, (keyPitch + wheelPitch + layer->pitch.get()) / 12.0f)
                                  * layer->sampleRate / getSampleRate();

        while (renderFrame < numSamples)
        {
            if (frame + renderFrame >= layer->getStart())
            {
                const int32 layerFrame = static_cast<int32> (layerPosition [i]);
                if (layerFrame < layer->out.get() && layerFrame < layer->getLength() && layerFrame < layer->lengthInSamples)
                {
                    const float alpha = (float) (layerPosition [i] - layerFrame);
                    const float invAlpha = 1.0f - alpha;

                    // just using a very simple linear interpolation here..
                    float l = (inL [layerFrame] * invAlpha + inL [layerFrame + 1] * alpha);
                    float r = (inR != nullptr) ? (inR [layerFrame] * invAlpha + inR [layerFrame + 1] * alpha) : l;

                    l *= (key.gain * layer->gain.get());
                    r *= (key.gain * layer->gain.get());

                   #if KSP1_USE_PANNING
                    l *= std::sqrt (layer->panning.get());
                    r *= std::sqrt (1.0f - layer->panning.get());
                   #endif

                    outL [renderFrame] += l;
                    outR [renderFrame] += r;
                }
            }

            layerPosition[i] += pitchRatio;
            ++renderFrame;
        }

        renderFrame = 0;
    }

    // done with the sound
    sound->release();

    // copy the rendering into the fx bus slots
    const int32 numOuts = outputBuffer.getNumChannels();
    int32 renderChannel = 0;
    int32 gainIndex = 0;
    float gain = 1.0f;

    for (int chan = 2; chan < numOuts; ++chan)
    {
        if (chan < 2)
        {
            gainIndex = 0;
            gain = key.gain;
        }
        else if (chan < 4)
        {
            gainIndex = 1;
            gain = key.fxGain [0];
        }
        else if (chan < 6)
        {
            gainIndex = 2;
            gain = key.fxGain [1];
        }
        else if (chan < 8)
        {
            gainIndex = 3;
            gain = key.fxGain [2];
        }
        else if (chan < 10)
        {
            gainIndex = 4;
            gain = key.fxGain [3];
        }


        outputBuffer.addFromWithRamp (chan, startSample,
                                      outputBuffer.getReadPointer (renderChannel, startSample),
                                      numSamples, lastGains [gainIndex], gain);

        if (++renderChannel == 2)
            renderChannel = 0;

        lastGains [gainIndex] = gain;
    }

    frame = endFrame;

    if (isLastCycle) {
        setPlaybackState (Stopped);
    }
}

void SamplerVoice::stopNote (float,  bool allowTailOff)
{
    const KeyInfo& key (sound->getKeyInfo());

    if (! allowTailOff || key.triggerMode == TriggerMode::Gate || key.triggerMode == TriggerMode::OneShotGate)
    {
        setPlaybackState (StopRequested);
    }
    else
    {

    }
}

void SamplerVoice::setPlaybackState (State s)
{
    if (state == s)
        return;

    if (StopRequested == s)
    {
        return setPlaybackState (Stopped);
    }
    else if (Stopped == s)
    {
        sound = nullptr;
        note  = -1;
        clearCurrentNote();
    }
    else if (Playing == s)
    {

    }
    else if (Releasing == s)
    {
    }
    else if (PlayRequested == s)
    {

    }

    state = s;
}



void SamplerVoice::_renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
    return _renderNextBlock (outputBuffer, startSample, numSamples);

#define BAIL_RENDER clearCurrentNote(); \
sound = nullptr; \
jassertfalse; \
return;

    if (sound == nullptr)
        return;

    bool isLastCycle = false;

    int endFrame = frame + numSamples;
    if (endFrame >= sound->length())
    {
        numSamples = endFrame - sound->length();
        endFrame   = frame + numSamples;
        isLastCycle = true;
    }

    if (! sound->acquire()) {
        BAIL_RENDER
    }

    const KeyInfo& key (sound->getKeyInfo());
    const int32 numLayers = sound->getNumLayers();

    float* outL = outputBuffer.getWritePointer (0, startSample);
    float* outR = outputBuffer.getNumChannels() > 1
                ? outputBuffer.getWritePointer (1, startSample) : nullptr;

   #if KSP1_USE_ADSR
    tempBuffer.setSize (1, numSamples, false, false, true);
    float* adsrGain = tempBuffer.getSampleData (adsrChannel, 0);
    adsr.processBuffer (numSamples, adsrGain);
   #endif

    const float keyPitch = static_cast<float> (key.pitch) + sound->getPitchOffsetForNote (this->note);

    uint32 renderFrame = 0;
    while (--numSamples >= 0)
    {
        for (int32 i = 0; i < numLayers; ++i)
        {
             const int32 pos = (int32) layerPosition [i];
             const LayerData* const layer = sound->getLayer (i);

             // inclusively check velocity range 0.0 to 1.0
            #if KSP1_USE_VELOCITY_RANGE
             if (! (layer->velocityRange.getStart() <= velocityNormal &&
                    velocityNormal <= layer->velocityRange.getEnd()))
                 continue;
            #endif

             const float* const inL = layer->getSampleData (0, 0);
             const float* const inR = layer->numChannels > 1  ? layer->getSampleData (1, 0) : nullptr;

            if (! inL || pos >= layer->out.get() || pos >= layer->lengthInSamples) {
               #if 0
                DBG ("Break on layer");
                bool isNull = inL == nullptr;
                DBG ("inL: " << isNull << " pos: " << pos << " outPoint: " << layer->out.get() << " len: " << layer->lengthInSamples);
               #endif
                continue;
            }

             const double pitchRatio = pow (2.0, (keyPitch + wheelPitch + layer->pitch.get()) / 12.0f)
                                       * layer->sampleRate / getSampleRate();

             const float alpha = (float) (layerPosition [i] - pos);
             const float invAlpha = 1.0f - alpha;

             // just using a very simple linear interpolation here..
             float l = (inL [pos] * invAlpha + inL [pos + 1] * alpha);
             float r = (inR != nullptr) ? (inR [pos] * invAlpha + inR [pos + 1] * alpha)
                                        : l;

             l *= key.gain * layer->gain.get();
             r *= key.gain * layer->gain.get();

            #if KSP1_USE_PANNING
             const float pr = std::sqrt (layer->panning.get());
             const float pl = std::sqrt (1.0f - layer->panning.get());
             l *= pl;
             r *= pr;
            #endif

             outL [renderFrame] += l;
             outR [renderFrame] += r;

             layerPosition[i] += pitchRatio;
        }

       #if KSP1_USE_ADSR
        outL [renderFrame] *= adsrGain [renderFrame];
        outR [renderFrame] *= adsrGain [renderFrame];
       #endif

       #if KSP1_USE_VELOCITY_GAIN
        outL [renderFrame] *= (velocityGain);
        outR [renderFrame] *= (velocityGain);
       #endif

        ++renderFrame;
    }

    // done with the sound
    sound->release();

    // copy the rendering into the fx bus slots
    const int32 numOuts = outputBuffer.getNumChannels();
    int32 renderChannel = 0;
    int32 gainIndex = 0;
    float gain = 1.0f;

    for (int chan = 2; chan < numOuts; ++chan)
    {
        if (chan < 2)
        {
            gainIndex = 0;
            gain = key.gain;
        }
        else if (chan < 4)
        {
            gainIndex = 1;
            gain = key.fxGain [0];
        }
        else if (chan < 6)
        {
            gainIndex = 2;
            gain = key.fxGain [1];
        }
        else if (chan < 8)
        {
            gainIndex = 3;
            gain = key.fxGain [2];
        }
        else if (chan < 10)
        {
            gainIndex = 4;
            gain = key.fxGain [3];
        }


        outputBuffer.addFromWithRamp (chan, startSample,
                                      outputBuffer.getReadPointer (renderChannel, startSample),
                                      numSamples, lastGains [gainIndex], gain);

        if (++renderChannel == 2)
            renderChannel = 0;

        lastGains [gainIndex] = gain;
    }

    frame = endFrame;

    if (isLastCycle) {
        setPlaybackState (Stopped);
    }
}

}
