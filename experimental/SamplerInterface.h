/*
    SamplerInterface.h - This file is part of KSP1
    Copyright (C) 2014  Kushview, LLC.  All rights reserved.

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

#ifndef KSP1_INTERFACE_H
#define KSP1_INTERFACE_H

#include "Instrument.h"
#include "KSP1.h"

namespace KSP1 {
class KeyItem;
class LayerItem;

/** An interface for controlling the sampler */
class SamplerInterface : private ValueTree::Listener {
public:
    virtual ~SamplerInterface() { state.removeListener (this); }
    virtual const String& getUri() const { return String::empty; }

    void setInstrument (InstrumentPtr i);
    InstrumentPtr getInstrument() const { return instrument; }

    virtual void reloadState();
    virtual void handleMidi (const MidiMessage&) {}
    virtual void loadFile (const File&) {}

    virtual void getKeyForNote (const int note);

protected:
    SamplerInterface()
        : state (ValueTree::invalid) {
        state.addListener (this);
    }

    virtual bool addKey (const KeyItem&)        = 0;
    virtual bool removeKey (const KeyItem&)     = 0;
    virtual bool addLayer (const LayerItem&)    = 0;
    virtual bool removeLayer (const LayerItem&) = 0;
    virtual void clearKeyboard()                = 0;

    virtual void mutate (const Instrument& key, const Identifier& prop) = 0;
    virtual void mutate (const KeyItem& key, const Identifier& prop)    = 0;
    virtual void mutate (const LayerItem& key, const Identifier& prop)  = 0;

private:
    friend class ValueTree;
    InstrumentPtr instrument;
    ValueTree state;
    void valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property);
    void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded);
    void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved);
    void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex);
    void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged);
    void valueTreeRedirected (ValueTree& treeWhichHasBeenChanged);
};
} // namespace KSP1

#endif // KSP1_SAMPLER_INTERFACE_H
