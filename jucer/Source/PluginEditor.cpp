/*
    PluginEditor.cpp - This file is part of KSP1

    Copyright (C) 2013-2019 Kushview, LLC  All rights reserved.
      * Michael Fisher <mfisher@kushview.net>
*/

#include "editor/SamplerView.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace KSP1 {

PluginEditor::PluginEditor (PluginProcessor* plug, PluginWorld& pw)
    : AudioProcessorEditor (plug),
        world (pw)
{
    setLookAndFeel (&style);
    setOpaque (true);
    
    view.reset (new SamplerView ());
    addAndMakeVisible (view.get());
    setSize (view->getWidth(), view->getHeight());
    view->setInstrument (plug->getInstrument());
    plug->addChangeListener (this);
}

PluginEditor::~PluginEditor()
{
    processor.editorBeingDeleted (this);

    if (auto* const base = dynamic_cast<PluginProcessor*> (&processor))
    {
        base->removeChangeListener (this);
        base->unregisterEditor (this);
    }
    else
    {
        jassertfalse; // should not ever happen
    }
    
    setLookAndFeel (nullptr);
}

MidiKeyboardState& PluginEditor::getMidiKeyboardState()
{
    jassert (view);
    return view->getMidiKeyboardState();
}

void PluginEditor::changeListenerCallback (ChangeBroadcaster*)
{
    if (PluginProcessor* plugin = dynamic_cast<PluginProcessor*> (&processor))
        view->setInstrument (plugin->getInstrument());
}

void PluginEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}

void PluginEditor::resized()
{
    view->setBounds (getLocalBounds());
}

}
