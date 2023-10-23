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

#pragma once

#include "editor/Screens.h"
#include "editor/Waveform.h"

namespace KSP1 {

class LayersTimeline;
class SoundsTimeline;

class EditScreen : public Screen,
                   public Timer,
                   public Button::Listener {
public:
    EditScreen (SamplerDisplay& owner);
    ~EditScreen();

    void setPropertiesVisible (bool);

    void keySelectedEvent (const KeyItem&) override;
    void onDisplayUpdate() override;

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;

    void parentHierarchyChanged() override;

private:
    OwnedArray<AudioThumbnail> peaks;
    int lastNote;
    void timerCallback() override;
    void updateComponents();

    std::unique_ptr<LayersTimeline> timeline;
    std::unique_ptr<SoundsTimeline> sounds;
    TextButton addButton, removeButton;
    PropertyPanel properties;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditScreen)
};

} // namespace KSP1
