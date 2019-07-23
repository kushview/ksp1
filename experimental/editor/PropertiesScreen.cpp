/*
    PropertiesScreen.cpp - This file is part of KSP1
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

#include "editor/PropertyPage.h"
#include "editor/Screens.h"
#include "editor/MidiNoteSliderPropertyComponent.h"
#include "editor/PropertiesScreen.h"
#include "Instrument.h"

namespace KSP1 {



#if 0
    class ClipRangePropertyComponent : public PropertyComponent,
                                   private ValueListener
    {
    public:
        ClipRangePropertyComponent (const Value& _start, const Value& _length, const String& name,
                                double rangeMin = 0, double rangeMax = 1, double interval = 0.001)
            : PropertyComponent (name),
              start (), end(), length (), slider()
        {
            start.referTo (_start);
            length.referTo (_length);
            end.setValue ((double) start.getValue() + (double) length.getValue());
            end.referTo (slider.getMaxValueObject());

            addAndMakeVisible (&slider);
            slider.setRange (rangeMin, rangeMax, interval);

            slider.getMinValueObject().referTo (start);
            start.addListener (this);
            end.addListener (this);
            slider.setSliderStyle (Slider::TwoValueHorizontal);
            slider.setTextBoxStyle (Slider::TextBoxRight, true, 30, 18);
        }

        ~ClipRangePropertyComponent()
        {
            start.removeListener (this);
            end.removeListener (this);
        }

        void refresh() {

        }

    protected:
        Slider slider;
        Value start, end, length;

    private:
        friend class ValueListener;
        void valueChanged (Value& value)
        {
            length.setValue ((double) end.getValue() - (double) start.getValue());
            DBG ("len: " << length.getValue().toString());
        }

    };
#endif



    class KeyPropertyPage :  public PropertyPage
    {
    public:
        KeyPropertyPage()  { }
        ~KeyPropertyPage() { }

        void setKeyItem (const KeyItem& key)
        {
            getPanel().clear();

            StringArray choices;
            Array<var> values;
            Array<PropertyComponent*> props;
            ValueTree n = key.node();

           #if JUCE_DEBUG
            props.add (new TextPropertyComponent (n.getPropertyAsValue (Slugs::id, nullptr), "Object ID", 64, false));
            props.getLast()->setEnabled (false);
           #endif
            props.add (new MidiNoteSliderPropertyComponent (n.getPropertyAsValue (Slugs::note, nullptr), "Root Note"));
            props.add (new SliderPropertyComponent (n.getPropertyAsValue (Slugs::length, nullptr), "Key Span (semitones)", 0, 127, 1));
            props.add (new SliderPropertyComponent (n.getPropertyAsValue (Slugs::volume, nullptr), "Volume (dB)", -70, 12, 0.001));
            props.add (new SliderPropertyComponent (n.getPropertyAsValue (Slugs::pitch, nullptr), "Pitch (semitones)", -12, 12, 0.01));
            props.add (new SliderPropertyComponent (n.getPropertyAsValue (Tags::voiceGroup, nullptr), "Voice Group", -1, 7, 1));
            for (int i = 0; i < TriggerMode::numModes; ++i)
            {
                TriggerMode tm (i);
                choices.add (tm.getName());
                values.add (i);
            }
            props.add (new ChoicePropertyComponent (n.getPropertyAsValue (Tags::triggerMode, nullptr),
                                                    "Trigger Mode", choices, values));

#if KSP1_ARTICULATION
            n = n.getChildWithName (Tags::articulation);

            choices.clearQuick(); values.clearQuick();
            choices.add ("Single Stroke"); values.add ("singleStroke");
            choices.add ("Double Stroke"); values.add ("doubleStroke");
            choices.add ("Roll"); values.add ("roll");
            props.add (new ChoicePropertyComponent (n.getPropertyAsValue (Slugs::type, nullptr),
                                                  "Stroke Type", choices, values));
#endif
            getPanel().addSection ("Sound Properties", props);
            props.clearQuick();

            const int nlayers = key.countLayers();
            for (int i = 0; i < nlayers; ++i)
            {
                n = key.getLayer(i).node();
                if (! n.isValid())
                    continue;

               #if JUCE_DEBUG
                props.add (new TextPropertyComponent (n.getPropertyAsValue (Slugs::id, nullptr), "Object ID", 64, false));
                props.getLast()->setEnabled (false);
               #endif
                if (n.hasProperty(Slugs::file)) {
                    props.add (new TextPropertyComponent (n.getPropertyAsValue (Slugs::file, nullptr), "File", 256, false));
                    props.getLast()->setEnabled (false); // don't allow changing file (yet)
                                                         // to do this we need a custom PropertyComponent
                                                         // that contains a filechooser box
                }
                props.add (new SliderPropertyComponent (n.getPropertyAsValue (Slugs::volume, nullptr), "Volume (dB)", -40, 24, 0.01));
                props.add (new SliderPropertyComponent (n.getPropertyAsValue (Tags::panning, nullptr), "Panning (0 hard left, 1 hard right)", 0.0, 1.0, 0.01));
                props.add (new SliderPropertyComponent (n.getPropertyAsValue (Slugs::pitch, nullptr), "Pitch (semitones)", -12, 12, 0.01));

                String labelText ("Layer "); labelText << i << " Properties";
                getPanel().addSection (labelText, props);
                props.clearQuick();
            }
        }
    };



KitScreen::KitScreen (SamplerDisplay& d)
    : Screen (d, "Kit / Instrument", Screen::kitScreen)
{
    addPage ("All", new KeyPropertyPage ());
}

KitScreen::~KitScreen() { }


void KitScreen::keySelectedEvent (const KeyItem& key)
{
    if (KeyPropertyPage* const kpp = findPage<KeyPropertyPage>())
        kpp->setKeyItem (key);
}

}
