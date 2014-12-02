/*
    PluginEditor.cpp - This file is part of KSP1

    Copyright (C) 2013 Kushview, LLC  All rights reserved.
      * Michael Fisher <mfisher@kushview.net>
*/

#include "editor/SamplerEditor.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace KSP1 {
namespace Gui {

    PluginEditor::PluginEditor (PluginProcessor* plug)
        : AudioProcessorEditor (plug)
    {
        setLookAndFeel (&style);
        setOpaque (true);
        
        addAndMakeVisible (editor = new KSP1::Gui::SamplerEditor (plug->getSampler()));
        editor->setOpaque (true);

        setSize (editor->getWidth(), editor->getHeight());
    }
    
    PluginEditor::~PluginEditor()
    {
        AudioProcessor* base = getAudioProcessor();
        if (PluginProcessor* proc = dynamic_cast<PluginProcessor*> (base))
            if (KSP1::SamplerProcessor* samp = proc->getSampler())
                samp->editorBeingDeleted (editor);

        if (base)
            base->editorBeingDeleted (this);
        
        editor = nullptr;
        setLookAndFeel (nullptr);
    }
    
    void PluginEditor::setInstrument (InstrumentPtr i)
    {
        editor->setInstrument (i);
    }

    void PluginEditor::paint (Graphics& g)
    {
        g.fillAll (Colour (0xff333333));
    }
    
    void PluginEditor::resized()
    {
        if (editor)
            editor->setSize (getWidth(), getHeight());
    }
    
}}
