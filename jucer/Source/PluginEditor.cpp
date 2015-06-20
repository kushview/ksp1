/*
    PluginEditor.cpp - This file is part of KSP1

    Copyright (C) 2013 Kushview, LLC  All rights reserved.
      * Michael Fisher <mfisher@kushview.net>
*/

#include "../../libs/lvtk/lvtk/ui.hpp"
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "editor/LV2Editor.h"

namespace KSP1 {
namespace Gui {

    class PluginEditor::Module
    {
    public:
        Module (PluginProcessor* proc, PluginWorld& pw)
            : desc (&lvtk::get_lv2g2g_descriptors()[0])
        {
            jassert (desc);
            editor = nullptr;
            handle = nullptr;
            widget = nullptr;
            features.add (pw.createMapFeature());
            features.add (pw.createUnmapFeature());
            handle = desc->instantiate (desc, KSP1_URI, "",
                &Module::writeCallback, 
                static_cast<LV2UI_Controller> (proc),
                &widget, features);
            editor = static_cast <LV2Editor*> (handle);
        }

        ~Module()
        {
            if (handle)
                desc->cleanup (handle);
            widget = nullptr;
            editor = nullptr;
            handle = nullptr;
        }

        SamplerView* getView() { return (haveInstance()) ? static_cast<SamplerView*> (widget) : nullptr; }

        bool haveInstance() const { return (handle && editor && widget); }
        
        static void writeCallback (LV2UI_Controller controller,
                                   uint32_t         portIndex,
                                   uint32_t         bufferSize,
                                   uint32_t         portProtocol,
                                   const void*      buffer)
        {
            PluginProcessor* proc = static_cast<PluginProcessor*> (controller);
            proc->writeToPort (portIndex, bufferSize, portProtocol, buffer);
        }
         
    private:
        const LV2UI_Descriptor* desc;
        LV2UI_Widget widget;
        LV2UI_Handle handle;
        LV2FeatureArray features;
        LV2Editor* editor;
    };

    PluginEditor::PluginEditor (PluginProcessor* plug, PluginWorld& pw)
        : AudioProcessorEditor(plug),
          world (pw)
    {
        setLookAndFeel(&style);
        setOpaque(true);
        module = new Module(plug, pw);
        if (SamplerView* view = module->getView())
        {
            addAndMakeVisible (view);
            setSize (view->getWidth(), view->getHeight());
        }
    }
    
    PluginEditor::~PluginEditor()
    {
        if (AudioProcessor* base = getAudioProcessor())
            base->editorBeingDeleted (this);

        if (SamplerView* view = module->getView())
            removeChildComponent (view);

        module = nullptr;
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
        module->getView()->setBounds (getLocalBounds());
    }    
}}
