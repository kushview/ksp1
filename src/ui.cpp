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
#include <lvtk/options.hpp>
#include <lvtk/ui.hpp>

#include <lvtk/ext/idle.hpp>
#include <lvtk/ext/parent.hpp>
#include <lvtk/ext/resize.hpp>
#include <lvtk/ext/urid.hpp>

#include "ports.hpp"
#include "urids.hpp"

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
        : UI (args) {
        for (const auto& opt : lvtk::OptionArray (options())) {
            if (opt.key == map_uri (LV2_UI__scaleFactor))
                m_scale_factor = *(float*) opt.value;
        }
    }

    void cleanup() {
    }

    int idle() {
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
        return (LV2UI_Widget) nullptr;
    }

private:
    float m_scale_factor { 1.f };
};

static lvtk::UIDescriptor<KSP1UI> _KSP1UI (
    KSP1_UI_URI, { LV2_UI__parent });
