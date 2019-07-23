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

    GUI class created by the Introjucer!
*/

#ifndef KSP1_EDIT_SCREEN_H
#define KSP1_EDIT_SCREEN_H

//[Headers]
#include "editor/LayersListBox.h"
#include "editor/Screens.h"
#include "editor/Waveform.h"

namespace KSP1 {

class LayersTimeline;
class SoundsTimeline;

class EditScreen  : public Screen,
                    public Timer,
                    public ButtonListener
{
public:
    EditScreen (SamplerDisplay& owner);
    ~EditScreen();

    void keySelectedEvent (const KeyItem&);
    void onDisplayUpdate();

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    Screen::ID type() const { return Screen::editScreen; }

private:
    OwnedArray<AudioThumbnail>   peaks;
    int lastNote;
    void timerCallback();
    void updateComponents();

    class Updater;
    friend class Updater;
    ScopedPointer<Updater> updater;
    ScopedPointer<LayersTimeline> timeline;
    ScopedPointer<SoundsTimeline> sounds;

    ScopedPointer<TextButton> buttonAddSample;
    ScopedPointer<TextButton> buttonRemoveLayer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditScreen)
};


} 

#endif   // KSP1_EDIT_SCREEN_H
