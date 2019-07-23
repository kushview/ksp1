/*
    PluginEditor.cpp - This file is part of KSP1

    Copyright (C) 2013 Kushview, LLC  All rights reserved.
      * Michael Fisher <mfisher@kushview.net>
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace KSP1 {

PluginEditor::PluginEditor (PluginProcessor* plug, PluginWorld& pw)
    : AudioProcessorEditor (plug),
        world (pw)
{
    setLookAndFeel (&style);
    setOpaque (true);
}

PluginEditor::~PluginEditor()
{
    if (PluginProcessor* base = dynamic_cast<PluginProcessor*> (getAudioProcessor()))
    {
        base->unregisterEditor (this);
        base->editorBeingDeleted (this);
    }
    
    setLookAndFeel (nullptr);
}

void PluginEditor::setInstrument (InstrumentPtr i)
{
    // FIXME: editor->setInstrument (i);
}

void PluginEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}

void PluginEditor::resized()
{
}

}
