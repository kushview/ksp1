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
    
    InstrumentPtr instrument = new Instrument ("Instrument");
    view->setInstrment (instrument);
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

void PluginEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}

void PluginEditor::resized()
{
    view->setBounds (getLocalBounds());
}

}
