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

#include "editor/AudioPeaks.h"
#include "editor/Waveform.h"
#include "InstrumentLoader.h"

namespace KSP1 {

class Waveform::Sink :  public ProgressSink
{
public:
    Sink (Waveform& w) : wave(w) { }
    void handleProgress (float p)
    {

    }

    void handleStatus (const String& msg)
    {

    }

    void handleProgressStart()
    {

    }

    void handleProgressFinished()
    {

    }


private:
    Waveform& wave;
};

Waveform::Waveform()
{
    sink = new Sink (*this);
}

Waveform::~Waveform()
{
    peak = nullptr;
    sink = nullptr;
}

void Waveform::setAudioPeak (AudioPeakPtr nextPeak)
{
    if (peak == nextPeak)
        return;

    AudioPeakPtr oldPeak = peak;

    if (oldPeak)
        oldPeak->removeChangeListener (this);

    if (nextPeak) {
        nextPeak->addChangeListener (this);
        timeSpan.setEnd (nextPeak->getTotalLength ());
    }

    peak = nextPeak;
    oldPeak = nullptr;
}

AudioPeakPtr Waveform::audioPeak()
{
    return peak;
}

void Waveform::paint (Graphics& g)
{
    g.fillAll (Colours::white);
    g.setColour (Colours::lightgreen);

    if (peak && timeSpan.getLength() > 0)
    {
        peak->drawChannels (g, getLocalBounds().reduced (2),
                                timeSpan.getStart(),
                                timeSpan.getEnd(),
                                1.0f);
    }
    else
    {
        g.setFont (14.0f);
        g.drawFittedText ("No data", getLocalBounds(), Justification::centred, 2);
    }
}

void Waveform::changeListenerCallback (ChangeBroadcaster* source)
{
    if (peak && (void*) peak.get() == (void*) source) {
        repaint();
    }
}

}
