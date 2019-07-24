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

#include "editor/MidiKeyboardComponent.h"

namespace KSP1 {
    class KeyboardWidget;

    class KeySelectedTimer : public Timer
    {
    public:
        KeySelectedTimer (KeyboardWidget& k);
        inline void timerCallback();

    private:
        KeyboardWidget& keyboard;
        int lastSentKey;

    };


    /** Keyboard extension class.
        This one extends the juce keyboard component by adding boost signals
        for event notification and changes.
     */
    class KeyboardWidget : public DragAndDropTarget,
                           public KSP1::MidiKeyboardComponent
    {
    public:

        KeyboardWidget (MidiKeyboardState& state)
            : MidiKeyboardComponent (state, MidiKeyboardComponent::horizontalKeyboard),
              selectedTimer (*this), lastPressedNote (-1)
        {
            setKeyPressBaseOctave (0);
            setWantsKeyboardFocus (true);
            setKeyWidth (24);
            selectedTimer.startTimer (300);
        }

        KeyboardWidget (MidiKeyboardState& state, Orientation orientation)
            : MidiKeyboardComponent (state, orientation),
              selectedTimer (*this), lastPressedNote (-1)
        {
            setKeyPressBaseOctave (0);
            setWantsKeyboardFocus (true);
            selectedTimer.startTimer (300);
        }

        ~KeyboardWidget()
        {
            selectedTimer.stopTimer();
        }

        inline int
        getSelectedKey() const
        {
            return lastPressedNote;
        }

        inline boost::signals2::signal<void(const MidiMessage&)>&
        signalMidi() { return midiSignal; }

        inline boost::signals2::signal<void(int)>&
        signalKeySelected() { return keySelectedSignal; }


        // Drag and Drop
        bool isInterestedInDragSource (const SourceDetails& dragSourceDetails) override;
        void itemDragEnter (const SourceDetails& dragSourceDetails) override;
        void itemDragMove (const SourceDetails& dragSourceDetails) override;
        void itemDragExit (const SourceDetails& dragSourceDetails) override;
        void itemDropped (const SourceDetails& dragSourceDetails) override;
        bool shouldDrawDragImageWhenOver() override;

    protected:

        inline void mouseUp (const MouseEvent& ev) override
        {
            MidiKeyboardComponent::mouseUp (ev);
            keySelectedSignal (lastPressedNote);
        }

        inline void mouseUpOnKey (int n, const MouseEvent& e) override
        {

        }

        inline bool keyPressed (const KeyPress& key) override
        {
            const int code = key.getKeyCode();

            if (KeyPress::F1Key == code)
                setKeyPressBaseOctave (0);
            else if (KeyPress::F2Key == code)
                setKeyPressBaseOctave (1);
            else if (KeyPress::F3Key == code)
                setKeyPressBaseOctave (2);
            else if (KeyPress::F4Key == code)
                setKeyPressBaseOctave (3);
            else if (KeyPress::F5Key == code)
                setKeyPressBaseOctave (4);
            else if (KeyPress::F6Key == code)
                setKeyPressBaseOctave (5);
            else if (KeyPress::F7Key == code)
                setKeyPressBaseOctave (6);
            else if (KeyPress::F8Key == code)
                setKeyPressBaseOctave (7);
            else if (KeyPress::F9Key == code)
                setKeyPressBaseOctave (8);
            else if (KeyPress::F10Key == code)
                setKeyPressBaseOctave (9);
            else if (KeyPress::F11Key == code)
                setKeyPressBaseOctave (10);

            return MidiKeyboardComponent::keyPressed (key);
        }

    private:
        friend class KeySelectedTimer;
        KeySelectedTimer selectedTimer;

        boost::signals2::signal<void(const MidiMessage&)> midiSignal;
        boost::signals2::signal<void(int)> keySelectedSignal;

        int lastPressedNote;

        inline void handleNoteOn (MidiKeyboardState* s, int chan, int note, float velocity) override
        {
            lastPressedNote = note;
            midiSignal (MidiMessage::noteOn (chan, note, velocity));

            MidiKeyboardComponent::handleNoteOn (s, chan, note, velocity);
        }

        inline void handleNoteOff (MidiKeyboardState* s, int chan, int note)
        {
            midiSignal (MidiMessage::noteOff (chan, note));
            MidiKeyboardComponent::handleNoteOff (s, chan, note, 0.0f);
        }
    };

    inline KeySelectedTimer::KeySelectedTimer (KeyboardWidget &k)
        : keyboard (k), lastSentKey(-1) { }

    inline void KeySelectedTimer::timerCallback()
    {
        if (lastSentKey != keyboard.lastPressedNote)
        {
            keyboard.keySelectedSignal (keyboard.lastPressedNote);
            lastSentKey = keyboard.lastPressedNote;
        }
    }
}
