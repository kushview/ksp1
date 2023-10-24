/*
    This file is part of Roboverb

    Copyright (C) 2015-2023  Kushview, LLC.  All rights reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <algorithm>
#include <iostream>

#include <lvtk/memory.hpp>

#include <lvtk/ui.hpp>
#include <lvtk/ui/button.hpp>
#include <lvtk/ui/opengl.hpp>
#include <lvtk/ui/slider.hpp>
#include <lvtk/ui/widget.hpp>

#include <lvtk/ext/idle.hpp>
#include <lvtk/ext/parent.hpp>
#include <lvtk/ext/resize.hpp>
#include <lvtk/ext/urid.hpp>

#include <lvtk/options.hpp>

#include "ports.hpp"

#define KSP1_UI_URI "https://kushview.net/plugins/ksp1/ui"

class Content : public lvtk::Widget {
public:
    std::function<void (uint32_t, float)> on_control_changed;

    Content() {
        set_opaque (true);
    }

    ~Content() {}

protected:
    void resized() override {}

    void paint (lvtk::Graphics& g) override {
        g.set_color (0xff242222);
        g.fill_rect (bounds().at (0));
        g.set_color (0xffffffff);
        g.draw_text ("KSP1",
                     bounds().at (0).smaller (3, 4).as<float>(),
                     lvtk::Align::CENTERED);
    }
};

struct ScopedFlag {
    ScopedFlag (bool& val, bool set) : original (val), value (val) {
        value = set;
    }
    ~ScopedFlag() { value = original; }

private:
    const bool original;
    bool& value;
};

class KSP1UI final : public lvtk::UI<KSP1UI, lvtk::Parent, lvtk::Idle, lvtk::URID, lvtk::Options> {
public:
    KSP1UI (const lvtk::UIArgs& args)
        : UI (args),
          _main (lvtk::Mode::MODULE, std::make_unique<lvtk::OpenGL>()) {
        for (const auto& opt : lvtk::OptionArray (options())) {
            if (opt.key == map_uri (LV2_UI__scaleFactor))
                m_scale_factor = *(float*) opt.value;
        }

        widget();
    }

    void cleanup() {
        content.reset();
    }

    int idle() {
        _main.loop (0);
        return 0;
    }

    bool _block_sending { false };

    void send_control (uint32_t port, float value) {
        if (_block_sending)
            return;
        write (port, value);
    }

    void port_event (uint32_t port, uint32_t size, uint32_t format, const void* buffer) {
        if (format != 0 || size != sizeof (float))
            return;

        ScopedFlag sf (_block_sending, true);
    }

    LV2UI_Widget widget() {
        if (content == nullptr) {
            content = std::make_unique<Content>();
            _main.elevate (*content, 0, (uintptr_t) parent.get());
            content->set_visible (true);
            content->on_control_changed = std::bind (
                &KSP1UI::send_control, this, std::placeholders::_1, std::placeholders::_2);
        }

        return (LV2UI_Widget) content->find_handle();
    }

private:
    float m_scale_factor { 1.f };
    lvtk::Main _main;
    std::unique_ptr<Content> content;
};

static lvtk::UIDescriptor<KSP1UI> __eVerbUI (
    KSP1_UI_URI, { LV2_UI__parent });
