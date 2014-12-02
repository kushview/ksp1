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

#ifndef KSP1_SAMPLER_DIRECT_INTERFACE_H
#define KSP1_SAMPLER_DIRECT_INTERFACE_H

#if 1
#include "KSP1.h"

#include "Instrument.h"
#include "SamplerInterface.h"
#include "engine/SamplerSynth.h"
#include "Types.h"

namespace KSP1 {

    class LayerData;
    class SampleCache;
    class SampleItem;
    class SamplerSound;

    class DirectInterface : public SamplerSynth,
                            public SamplerInterface
    {
    public:

        const String& getUri() const { return uri; }
        DirectInterface (SampleCache& cache);
        virtual ~DirectInterface();

        void clearKeyboard() override;

#if 0
        void setInstrument (InstrumentPtr);
        InstrumentPtr getInstrument() const { return instrument; }
        void setState (const ValueTree& data);
        ValueTree getState() const { return state; }
#endif
        void reloadState();


    private:
        String uri;

        bool addKey (const KeyItem&) { return true; }
        bool removeKey (const KeyItem&) { return true; }
        bool addLayer (const LayerItem& layer);
        bool removeLayer (const LayerItem& layer);

        /** Property mutation */
        void mutate (const Instrument& instrument, const Identifier& prop);
        void mutate (const KeyItem& key, const Identifier& prop);
        void mutate (const LayerItem& layer, const Identifier& prop);

#if 0
        friend class ValueTree;
        InstrumentPtr instrument;
        ValueTree state;
        void valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property);
        void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded);
        void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved);
        void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved);
        void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged);
        void valueTreeRedirected (ValueTree& treeWhichHasBeenChanged);
#endif

    };

}
#endif
#endif // KSP1_SAMPLER_SYNTH_H
