/*
    LV2Editor.h - This file is part of KSP1
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

#ifndef KSP1_LV2_EDITOR_H
#define KSP1_LV2_EDITOR_H

#include "KSP1.h"
#include "editor/SamplerView.h"

#include "../../libs/lvtk/lvtk/ui.hpp"
#include "../../libs/lvtk/lvtk/ext/atom.hpp"
#include "../../libs/lvtk/lvtk/ext/idle_interface.hpp"

namespace Element {
    class Style;
}

namespace KSP1 {
class PortWriterInterface;

namespace Gui {
class KeyboardGetter;
class AudioPeakFactory;
class DisplayView;

class LV2Editor;
typedef lvtk::UI<LV2Editor, lvtk::URID<true>,
                 lvtk::InstanceAccess<false>,
                 lvtk::UIResize<false>,
                 lvtk::Parent<false>,
                 lvtk::IdleInterface<false> > LV2EditorType;

class LV2Editor : public LV2EditorType
{
public:
    LV2Editor (const char* plugin);
    ~LV2Editor();
    void port_event (uint32_t port, uint32_t size, uint32_t format, void const* buffer);
    LV2UI_Widget widget();

    int idle();

    void write_to_port (uint32_t, uint32_t, uint32_t, void const*);

private:
    const std::string plugin_uri;
    intptr_t xwin, xwin_parent;
    bool runLoop;
    ScopedPointer<AudioPeakFactory> peaks;
    ScopedPointer<Forge> forge;
    ScopedPointer<KSP1::URIs>  uris;
    ScopedPointer<KeyboardGetter> keyboard;
    Element::LookAndFeel_E1 style;
    ScopedPointer<PortWriter> writer;
    ScopedPointer<DisplayView> view;
    ScopedPointer<PortWriterInterface> interface;
    DisplayView* create_view();
};

}}

#endif /* KSP1_LV2_EDITOR_H */
