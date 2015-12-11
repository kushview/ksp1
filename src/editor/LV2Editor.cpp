/*
    LV2Editor.cpp - This file is part of KSP1
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

#include <functional>

#include "editor/AudioPeaks.h"
#include "LV2Editor.h"
#include "PortWriterInterface.h"
#include "PortWriter.h"
#include "URIs.h"
#include "../../libs/lvtk/src/ui.cpp"

#if __linux__
 #if KSP1_STANDALONE
  #define KSP1_USE_LV2_MESSAGE_THREAD 0
 #else
  #define KSP1_USE_LV2_MESSAGE_THREAD 1
 #endif
#else
 #define KSP1_USE_LV2_MESSAGE_THREAD 0
#endif

namespace KSP1 {
namespace Gui {

// need to qualify these to avoid conflicts with other libraries (boost).
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;
using std::placeholders::_4;


class KeyboardGetter :  private Timer
{
public:
    KeyboardGetter (SamplerInterface& i)
        : iface(i), note (-1)
    {
        getKeyboard();
    }

    void getKeyboard()
    {
        if (isTimerRunning())
            return;

        note = 0;
        startTimer (30);
    }

private:
    SamplerInterface& iface;
    int note;
    void timerCallback()
    {
        if (isPositiveAndBelow (note, 128))
            iface.getKeyForNote (note);

        if (note < 0 || ++note == 128) {
            stopTimer();
            note = -1;
        }
    }

};


static Array<LV2Editor*> editors;

class LV2EditorInit : public DeletedAtShutdown {
public:
    LV2EditorInit()
    {
       #if KSP1_USE_LV2_MESSAGE_THREAD
        initialiseJuce_GUI();
        MessageManager::getInstance();
       #endif
    }

    ~LV2EditorInit() { }
};
static LV2EditorInit* editor_init = nullptr;


LV2Editor::LV2Editor (const char* plugin)
    : plugin_uri (plugin),
      xwin(0), xwin_parent(0),
      runLoop (false)
{
    if (! editor_init)
        editor_init = new LV2EditorInit ();

    peaks = new AudioPeakFactory();

    // setup the forge and URIs objects
    URIs::MapFunc map = std::bind (&LV2Editor::map, this, _1);
    uris  = new URIs (map);
    forge = new Forge (*uris, get_urid_map(), get_urid_unmap());
    create_view();

   #if KSP1_USE_LV2_MESSAGE_THREAD
    if (LV2UI_Widget* parent = this->get_parent())
        xwin_parent = (intptr_t) parent;

    if (view)
    {
        view->setVisible (true);
        view->addToDesktop (0, (void*) xwin_parent);
        ui_resize (view->getWidth(), view->getHeight());
        xwin = (intptr_t) view->getPeer()->getNativeHandle();
    }
   #endif
    
    if (editors.size() == 0)
        runLoop = true;

    editors.add (this);
    keyboard = new KeyboardGetter (*interface);
}

LV2Editor::~LV2Editor()
{
    editors.removeFirstMatchingValue (this);

    if (view) {
        view = nullptr;
    }

    if (interface) {
        interface = nullptr;
    }

    peaks = nullptr;

    if (editors.size() == 0)
    {
       #if KSP1_USE_LV2_MESSAGE_THREAD
        shutdownJuce_GUI();
       #endif
        editor_init = nullptr;
    }
    else if (runLoop)
    {
        for (LV2Editor* ed : editors)
            ed->runLoop = false;
        editors.getFirst()->runLoop = true;
    }
}

int LV2Editor::idle()
{
    if (! runLoop)
        return 0;

   #if KSP1_USE_LV2_MESSAGE_THREAD
    MessageManager::getInstance()->runDispatchLoopUntil (24);
   #endif

    return 0;
}

void LV2Editor::port_event (uint32_t port, uint32_t size, uint32_t format, void const* buffer)
{
    if (port == Port::AtomOutput && format == uris->atom_eventTransfer)
    {
        const lvtk::Atom atom (buffer);

        if (atom.total_size() != size) {
            return;
        }

        interface->setFrozen (true);

        if (atom.type() == uris->atom_Object && atom.has_object_type (333333))
        {
            const lvtk::Atom left, right;
            lv2_atom_object_get (atom.as_object().cobj(), 1, &left, 2, &right, 0);
            if (left && right) {
                view->setMainRMS (left.as_float(), right.as_float());
            }
        }
        else if (atom.type() == uris->atom_Object && atom.as_object().otype() == uris->patch_Response)
        {
            const AtomObject object (atom.as_object());
            const Atom body, request, sequenceNumber;
            lv2_atom_object_get (object, uris->patch_body, &body,
                                         uris->patch_request, &request,
                                         uris->patch_sequenceNumber, &sequenceNumber,
                                 0);
            DBG ("Got a patch_Response from the plugin");
        }

        else if (atom.type() == uris->atom_Object && atom.as_object().otype() == uris->patch_Ack) {
            DBG ("Got a patch_Ack from the plugin");
        }

        else if (atom.has_object_type (uris->ksp1_LayerData)) {
            interface->getInstrument()->updateLayerItem (*uris, atom.as_object());
        }

        else if (atom.has_object_type (uris->ksp1_SamplerSound)) {
            interface->getInstrument()->updateKeyItem (*uris, atom.as_object());
        }

        else if (atom.has_object_type (uris->ksp1_SamplerSynth)) {
            interface->getInstrument()->clear();
            view->stabilizeView();
            keyboard->getKeyboard();
        }

        else if (atom.type() == 100100)
        {
            // compressed JSON
            MemoryInputStream input (atom.body(), atom.size(), false);
            GZIPDecompressorInputStream decomp (input);
            const var data (JSON::parse (decomp));
            InstrumentPtr in = new Instrument ("Invalid");

            if (var::null != data && data.size() > 0)
            {
                for (int i = 0; i < data.size(); ++i) {
                    const var& child (data [i]);
                    if (child.isObject()) {

                    }
                }
            }
            else if (var::null != data && data.isObject())
            {
                if (DynamicObject* d = data.getDynamicObject())
                {
                    if (d->hasProperty (Slugs::volume))
                        in->setVolume ((float) d->getProperty (Slugs::volume));

                    const var sounds (d->getProperty (Identifier ("sounds")));
                    if (sounds != var::null)
                    {
                        for (int i = 0; i < sounds.size(); ++i)
                        {
                            const var& sound (sounds [i]);
                            DynamicObject* so = sound.getDynamicObject();
                            ValueTree keyData (Tags::key);

                            for (int j = 0; j < so->getProperties().size(); ++j)
                            {
                                const Identifier slug = so->getProperties().getName (j);
                                const var value = so->getProperties().getValueAt (j);

                                if (slug == Identifier ("layers"))
                                {
                                    for (int k = 0; k < value.size(); ++k)
                                    {
                                        const var& layer (value [k]);
                                        ValueTree layerData (Tags::layer);
                                        if (DynamicObject* lo = layer.getDynamicObject())
                                        {
                                            for (int l = 0; l < lo->getProperties().size(); ++l) {
                                                layerData.setProperty (
                                                    lo->getProperties().getName(l),
                                                    lo->getProperties().getValueAt(l),
                                                0);
                                            }
                                        }
                                        keyData.addChild (layerData, -1, 0);
                                    }
                                }
                                else
                                {
                                    keyData.setProperty (
                                           so->getProperties().getName(j),
                                           so->getProperties().getValueAt(j),
                                    0);
                                }
                            }
                            in->node().addChild (keyData, 0, 0);
                        }
                    }
                }
            }

            in->setMissingProperties (true);
            view->setInstrment (in);
        }
    }

    interface->setFrozen (false);
}

LV2UI_Widget LV2Editor::widget()
{
   #if KSP1_USE_LV2_MESSAGE_THREAD
    return reinterpret_cast<LV2UI_Widget> (xwin);
   #else
    return reinterpret_cast<LV2UI_Widget> (view.get());
   #endif
}

SamplerView* LV2Editor::create_view()
{
    if (nullptr == view)
    {
        view = new SamplerView();
        view->setLookAndFeel (&style);

        PortWriter::WriteFunc f = std::bind (&LV2Editor::write, this, _1, _2, _3, _4);
        writer = new PortWriter (*forge, f);
        interface = new PortWriterInterface (*writer);
        interface->setInstrument (view->getInstrument());
        view->setInterface (interface);
    }

    return view.get();
}


}}

const unsigned ksp1guii = KSP1::Gui::LV2Editor::register_class (KSP1_GUI_URI);
