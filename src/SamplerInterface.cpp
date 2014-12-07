/*
    SamplerInterface.cpp - This file is part of KSP1
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

#include "SamplerInterface.h"

namespace KSP1 {


void SamplerInterface::getKeyForNote (const int note) { }

void SamplerInterface::setInstrument (InstrumentPtr i)
{
    instrument = i;
    state = (i != nullptr) ? i->node() : ValueTree::invalid;
}

void SamplerInterface::valueTreePropertyChanged (ValueTree& tree, const Identifier& property)
{
    if (tree == state)
    {
        Instrument inst (tree);
        this->mutate (inst, property);
    }
    else if (tree.hasType (Tags::key))
    {
        KeyItem key (tree);
        this->mutate (key, property);
    }
    else if (tree.hasType (Tags::layer))
    {
        LayerItem layer (tree);
        this->mutate (layer, property);
    }
}

void SamplerInterface::valueTreeChildAdded (ValueTree& parent, ValueTree& child)
{
    if (child.hasType (Tags::key))
    {
        jassert (parent == state);
        const KeyItem key (child);
        this->addKey (key);
    }
    else if (child.hasType (Tags::layer))
    {
        const LayerItem layer (child);
        this->addLayer (layer);
    }
}

void SamplerInterface::valueTreeChildRemoved (ValueTree& parent, ValueTree& child)
{
    if (child.hasType (Tags::key))
    {
        const KeyItem key (child);
        this->removeKey (key);
    }
    else if (child.hasType (Tags::layer))
    {
        LayerItem layer (child);
        this->removeLayer (layer);
    }
}

void SamplerInterface::valueTreeChildOrderChanged (ValueTree& /*tree*/)
{

}

void SamplerInterface::valueTreeParentChanged (ValueTree& /*tree*/)
{

}

void SamplerInterface::valueTreeRedirected (ValueTree& tree)
{
    if (tree != state)
        return;

    reloadState();
}

void SamplerInterface::reloadState()
{
    clearKeyboard();
    Instrument i (state);

    for (int note = 0; note < 128; ++note)
    {
        KeyItem key (i.getKey (note, false));
        ValueTree kv = key.node();
        //const SamplerSound* const sound = getSound (key.note());
        //(void) sound;

        for (int i = kv.getNumProperties(); --i >= 0;)
        {
            const Identifier prop = kv.getPropertyName (i);
            mutate (key, prop);
        }

        for (int l = 0; l < key.countLayers(); ++l)
        {
            LayerItem layer (key.getLayer (l));
            addLayer (layer);

            ValueTree ld = layer.node();
            for (int32 i = ld.getNumProperties(); --i >= 0;)
            {
                const Identifier prop = ld.getPropertyName (i);
                mutate (layer, prop);
            }
        }
    }
}

}
