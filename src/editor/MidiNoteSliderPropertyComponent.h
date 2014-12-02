/*
    MidiNoteSliderPropertyComponent.h - This file is part of KSP1
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

#ifndef KSP1_MIDI_NOTE_SLIDER_PROPERTY_COMPONENT_H
#define KSP1_MIDI_NOTE_SLIDER_PROPERTY_COMPONENT_H

namespace KSP1 {

class MidiNoteSliderPropertyComponent : public SliderPropertyComponent,
                                        private ValueListener
{
public:
    MidiNoteSliderPropertyComponent (const Value& midiNoteValue, const String& name = "MIDI Note")
        : SliderPropertyComponent (midiNoteValue, name, 0.0, 127.0, 1.0)
    {
        slider.getValueObject().addListener (this);
        valueChanged (slider.getValueObject());
    }

    void setValue (double d) override {
        Value v (d);
        valueChanged (v);
    }

private:
    friend class ValueListener;
    inline void valueChanged (Value& value) override
    {
        String suffix = " (";
        suffix << MidiMessage::getMidiNoteName ((int) value.getValue(), true, true, 3)
               << ")";
        slider.setTextValueSuffix (suffix);
    }
};

}

#endif /* KSP1_MIDI_NOTE_SLIDER_PROPERTY_COMPONENT_H */
