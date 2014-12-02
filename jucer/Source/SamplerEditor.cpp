/*
    SamplerEditor.cpp - This file is part of KSP1
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

#include "editor/SamplerPluginView.h"
#include "editor/SamplerDisplay.h"
#include "editor/SamplerEditor.h"
#include "Globals.h"
#include "Sampler.h"

namespace KSP1 {
namespace Gui
{
    SamplerEditor::SamplerEditor (SamplerProcessor *s)
        : AudioProcessorEditor (s)
    {
        addAndMakeVisible (view = new KSP1::Gui::SamplerPluginView (*this));
        setSize (view->getWidth(), view->getHeight());
        // FIXME: view->display()->setInstrument (s->currentSynth()->getInstrument());
    }

    SamplerEditor::~SamplerEditor() { }

    Globals& SamplerEditor::globals() {
        return sampler()->globals();
    }

    void
    SamplerEditor::paint (Graphics& g)
    {
        g.setColour (Colour (0xff232323));
        g.fillAll();
    }

    void SamplerEditor::resized() {
        view->setBounds (getLocalBounds());
    }

    void SamplerEditor::setInstrument (InstrumentPtr i)
    {
        view->display()->setInstrument (i);
    }
}}
