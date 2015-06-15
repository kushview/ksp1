/*
    PluginEditor.cpp - This file is part of KSP1

    Copyright (C) 2013 Kushview, LLC  All rights reserved.
      * Michael Fisher <mfisher@kushview.net>
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace KSP1 {
namespace Gui {

    PluginEditor::PluginEditor (PluginProcessor* plug)
        : AudioProcessorEditor (plug)
    {
        setLookAndFeel (&style);
        setOpaque (true);
        
#if 0
        addAndMakeVisible (editor = new KSP1::Gui::SamplerEditor (plug->getSampler()));
        editor->setOpaque (true);

        setSize (editor->getWidth(), editor->getHeight());
#else
        setSize (400, 350);
#endif
    }
    
    PluginEditor::~PluginEditor()
    {
        AudioProcessor* base = getAudioProcessor();
#if 0
        if (PluginProcessor* proc = dynamic_cast<PluginProcessor*> (base))
            if (KSP1::SamplerProcessor* samp = proc->getSampler())
                samp->editorBeingDeleted (editor);
#endif
        if (base)
            base->editorBeingDeleted (this);
        
        setLookAndFeel (nullptr);
    }
    
    void PluginEditor::setInstrument (InstrumentPtr i)
    {
        // FIXME: editor->setInstrument (i);
    }

    void PluginEditor::paint (Graphics& g)
    {
        g.fillAll (Colour (0xff333333));
    }
    
    void PluginEditor::resized()
    {

    }    
}}
