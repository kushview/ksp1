/*
    SamplerEditor.h - This file is part of KSP1
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

#ifndef KSP1_SAMPLER_EDITOR_H
#define KSP1_SAMPLER_EDITOR_H

#include "Sampler.h"

namespace KSP1 {

    class Globals;
    class SamplerProcessor;

namespace Gui {

    class SamplerPluginView;

    /** A compact editor for the stand alone SamplerProcessor */
    class SamplerEditor :  public  AudioProcessorEditor
    {
    public:

        SamplerEditor (SamplerProcessor* s);
        virtual ~SamplerEditor();

        inline SamplerProcessor* sampler() const {
            return dynamic_cast<SamplerProcessor*> (this->getAudioProcessor());
        }

        Globals& globals();

        inline MidiKeyboardState& keyboard() {
            return sampler()->keyboard();
        }

        void setInstrument (InstrumentPtr i);
        void paint (Graphics& g) override;
        void resized() override;

    private:
        ScopedPointer<SamplerPluginView> view;

    };

} /* namespace Gui */
}
#endif // KSP1_SAMPLER_EDITOR_H
