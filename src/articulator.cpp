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

#include "juce.hpp"

#include "articulator.hpp"
#include "stroketype.hpp"

namespace ksp1 {

using juce::MidiBuffer;
using juce::MidiMessage;

class Articulator::Impl {
public:
    // The data needed to process a 'note' on a 'keyboard'
    class NoteData {
    public:
        NoteData (uint8_t n)
            : playing (false),
              stopRequested (false),
              triggered (false),
              beat (0.0f),
              enabled (true),
              interval (1.0f / 8.0f),
              legato (0.6f),
              type (StrokeType::SingleStroke) {
            midi = noteOn = MidiMessage::noteOn (1, (int) n, (uint8_t) 80);
            noteOff       = MidiMessage::noteOff (1, (int) n);
        }

        ~NoteData() {}

        const uint8_t* midiData() const noexcept { return midi.getRawData(); }

        void reset() noexcept {
            setType (StrokeType::Roll);
            enabled       = true;
            playing       = false;
            stopRequested = false;
            triggered     = false;
        }

        void setFrames (int64_t f) {
            frame = f;
        }

        void setType (StrokeType t) {
            type = t;
        }

        void setVelocity (uint8_t velocity) {
            midi.setVelocity ((float) velocity / 127.f);
        }

        // playback vars
        MidiMessage midi;
        MidiMessage noteOn;
        MidiMessage noteOff;
        bool playing { false },
            stopRequested { false },
            triggered { false };
        float beat { 0.f };
        int64_t frame { 0 };

        // properties
        bool enabled { true };
        float interval { 1.f / 8.f };
        float legato { 0.6f };
        StrokeType type { StrokeType::SingleStroke };
    };

    explicit Impl (double rate = 44100.0f, float bpm = 120.f)
        : samplerate (rate),
          rollButtonHeld (false) {
        data.reserve (128);
        for (uint8_t note = 0; note < 128; ++note)
            data.push_back (new NoteData (note));

        setSampleRate (rate);
        setTempo (bpm);
    }

    ~Impl() {
        for (NoteData* d : data)
            delete d;

        data.clear();
    }

    void setSampleRate (const double rate) {
        if (samplerate == rate)
            return;

        samplerate = rate;
        computeTimings();
    }

    void setTempo (const float bpm) {
        if (tempo == bpm)
            return;

        tempo = bpm;
        computeTimings();
    }

    void activate() {
        for (NoteData* d : data)
            d->reset();
    }

    void deactivate() {
    }

    void renderNotes (MidiBuffer& out, int start, int nframes) {
        for (NoteData* d : data) {
            if (! d->playing)
                continue;

            if (rollButtonHeld) {
                renderRoll (d, out, start, nframes);
                continue;
            }

            if (d->type == StrokeType::SingleStroke)
                renderSingleStroke (d, out, start, nframes);
            else if (d->type == StrokeType::DoubleStroke)
                renderDoubleStroke (d, out, start, nframes);
            else if (d->type == StrokeType::Roll)
                renderRoll (d, out, start, nframes);
        }
    }

    void renderDoubleStroke (NoteData* d, MidiBuffer& out, int start, int nframes) {
        renderSingleStroke (d, out, start, nframes); // xxx: this is not at all correct
    }

    void renderSingleStroke (NoteData* d, MidiBuffer& out, int start, int nframes) {
        juce::ignoreUnused (nframes);
        if (! d->triggered) {
            d->midi = MidiMessage::noteOn (d->midi.getChannel(),
                                           d->midi.getNoteNumber(),
                                           d->midi.getVelocity());
            d->setFrames (start);

            out.addEvent (d->midi, start);
            d->triggered = true;
        } else if (d->triggered && d->stopRequested) {
            d->midi = MidiMessage::noteOff (d->midi.getChannel(),
                                            d->midi.getNoteNumber());
            d->setVelocity (0x00);
            d->setFrames (start);
            out.addEvent (d->midi, start);

            d->triggered     = false;
            d->playing       = false;
            d->stopRequested = false;
        }
    }

    void renderRoll (NoteData* d, MidiBuffer& out, int start, int nframes) {
        const float delta    = beatsPerFrame * (float) nframes;
        const float duration = d->interval * d->legato;
        const float endbeat  = d->beat + delta;

        if (! d->triggered) {
            d->setFrames (start + llrint (framesPerBeat * d->beat));

            d->midi = MidiMessage::noteOn (d->midi.getChannel(),
                                           d->midi.getNoteNumber(),
                                           d->midi.getVelocity());

            out.addEvent (d->midi, d->frame);
            d->triggered = true;
        }

        if (d->triggered && endbeat >= duration && endbeat < duration + delta) {
            d->setFrames (llrint (framesPerBeat * (endbeat - duration)));
            d->midi = MidiMessage::noteOff (d->midi.getChannel(),
                                            d->midi.getNoteNumber(),
                                            d->midi.getVelocity());
            out.addEvent (d->midi, d->frame);
        }

        if (endbeat >= d->interval) {
            if (d->stopRequested) {
                d->triggered     = false;
                d->playing       = false;
                d->stopRequested = false;
            } else {
                d->beat = endbeat - d->interval;
                d->setFrames (nframes - llrint (d->beat * framesPerBeat));

                d->midi = MidiMessage::noteOn (d->midi.getChannel(),
                                               d->midi.getNoteNumber(),
                                               d->midi.getVelocity());

                out.addEvent (d->midi, d->frame);
            }
        } else {
            d->beat += delta;
        }
    }

    void process (MidiBuffer& input, int nframes) {
        int lastframe = 0, evFrame = 0;
        MidiMessage ev;

        for (const auto& m : input) {
            if (lastframe > nframes)
                break;

            ev      = m.getMessage();
            evFrame = m.samplePosition;

            renderNotes (output, lastframe, evFrame - lastframe);

            if (ev.isNoteOn()) {
                NoteData* const d = getNoteData (ev.getNoteNumber());
                if (d && d->enabled) {
                    d->midi.setVelocity (ev.getFloatVelocity());
                    d->midi.setChannel (ev.getChannel());
                    d->playing       = true;
                    d->stopRequested = false;
                    d->beat          = 0.0f;
                } else {
                    // pass thru
                    output.addEvent (ev, evFrame);
                }
            } else if (ev.isNoteOff()) {
                NoteData* const d = getNoteData (ev.getNoteNumber());
                if (d && d->enabled) {
                    d->midi.setChannel (ev.getChannel());
                    d->stopRequested = true;
                } else {
                    // pass thru
                    output.addEvent (ev, evFrame);
                }
            } else if (ev.isController()) {
#if KSP1_CONTROLLER_MODE
                if (midi.is_controller_type (LV2_MIDI_CTL_NRPN_LSB))
                    nrpn[0] = midi.controller_value();
                else if (midi.is_controller_type (LV2_MIDI_CTL_NRPN_MSB))
                    nrpn[1] = midi.controller_value();
                else if (midi.is_controller_type (LV2_MIDI_CTL_LSB_DATA_ENTRY)) {
                    if (nrpn[0] == 0x2D) {
                        clog << "Beat Thang [ROLL] " << (bool) midi.controller_value() << endl;
                        rollButtonHeld = (bool) midi.controller_value();
                    } else if (nrpn[0] >= 0x01 && nrpn[0] <= 0x0D) {
                        NoteData* const d = getNoteData (nrpn[0] - 0x01);

                        if (midi.controller_value() > 0) {
                            clog << "Pad ON: " << (int) nrpn[0] - 1 << endl;
                            d->playing       = true;
                            d->stopRequested = false;
                            d->beat          = 0.0f;
                        } else {
                            clog << "Pad OFF: " << (int) nrpn[0] - 1 << endl;
                            d->stopRequested = true;
                        }
                    }
                }
#endif
            } else {
                // not a note or controller event, pass thru
                output.addEvent (ev, evFrame);
            }

            lastframe = evFrame;
        }

        if (lastframe < nframes)
            renderNotes (output, lastframe, nframes - lastframe);

        input.swapWith (output);
        output.clear();
    }

    bool setStrokeType (int n, const StrokeType& t) {
        if (NoteData* nd = getNoteData (n)) {
            nd->setType (t);
            return true;
        }

        return false;
    }

private:
    MidiBuffer output;
    uint8_t nrpn[2];

    float beatsPerFrame;
    float framesPerBeat;
    double samplerate;
    float tempo;
    bool rollButtonHeld;

    std::vector<NoteData*> data;

    void computeTimings() {
        framesPerBeat = samplerate * 60.0f / tempo;
        beatsPerFrame = 1.0f / framesPerBeat;
    }

    NoteData* getNoteData (int note) {
        auto i = static_cast<size_t> (note);
        return note < 128 ? data[i] : nullptr;
    }
};

Articulator::Articulator() {
    impl = std::make_unique<Impl>();
}

Articulator::~Articulator() {}

void Articulator::prepareToPlay (double sampleRate, int blockSize) {
    juce::ignoreUnused (blockSize);
    impl->setSampleRate (sampleRate);
    impl->activate();
}

void Articulator::releaseResources() {
    impl->deactivate();
}

void Articulator::processMidi (MidiBuffer& midi, int nframes) {
    impl->process (midi, nframes);
}

bool Articulator::setStrokeType (int note, const StrokeType& type) {
    return impl->setStrokeType (note, type);
}

} // namespace ksp1
