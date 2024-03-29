/*
    This file is part of KSP1
    Copyright (C) 2014-2019  Kushview, LLC. All rights reserved.

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

#pragma once

#include <cassert>
#include <string>

#include "adsr.hpp"
#include "util.hpp"

namespace ksp1 {

class LayeringMode {
public:
    enum ID {
        Combine = 0,  ///< Layers are mixed (combined)
        RoundRobin,   ///< Layers are alternated upon each note-on
        VelocityRange ///< Layer is selected according to a velocity range
    };

    LayeringMode() : mode (Combine) {}

    LayeringMode (const int& m) {
        assert (m < VelocityRange + 1);
        mode = static_cast<ID> (m);
    }

    LayeringMode (const ID& id) : mode (id) {}

    LayeringMode (const std::string& id) {
        if (id == "combine") {
            mode = Combine;
        } else if (id == "roundRobin") {
            mode = RoundRobin;
        } else if (id == "combine") {
            mode = VelocityRange;
        } else {
            assert (false);
        }
    }

    const ID getMode() const { return static_cast<ID> (mode); }

    bool operator== (const LayeringMode& o) const { return o.mode == mode; }
    bool operator== (const int& o) const { return o == mode; }
    bool operator!= (const LayeringMode& o) const { return o.mode != mode; }
    bool operator!= (const int& o) const { return o != mode; }

    LayeringMode& operator= (const LayeringMode& o) {
        mode = o.mode;
        return *this;
    }

private:
    int mode;
};

class TriggerMode {
public:
    enum ID {
        Retrigger   = 0, ///< previous sounds are cutoff. Default.
        OneShot     = 1, ///< previous sounds continue
        Gate        = 2, ///< sample plays only until note off and loops
        OneShotGate = 3  ///< sample plays until note off and doesn't loop
    };

    static constexpr auto numModes = 4;

    TriggerMode() : type (Retrigger) {}
    TriggerMode (const int& id)
        : type ((ID) ksp1::limit ((int) Retrigger, (int) OneShotGate, (int) id)) {}
    TriggerMode (const ID& id) : type (id) {}
    TriggerMode (const TriggerMode& other) { type = other.type; }

    inline std::string getName() const {
        switch (type) {
            case Retrigger:
                return "Retrigger";
                break;
            case OneShot:
                return "One Shot";
                break;
            case Gate:
                return "Gate";
                break;
            case OneShotGate:
                return "One Shot Gate";
                break;
        }

        return "Unknown";
    }

    inline std::string getSlug() const {
        switch (type) {
            case Retrigger:
                return "retrigger";
                break;
            case OneShot:
                return "oneShot";
                break;
            case Gate:
                return "gate";
                break;
            case OneShotGate:
                return "oneShotGate";
                break;
        }

        return "Unknown";
    }

    inline const ID& id() const { return type; }
    inline operator int() const { return (int) type; }
    inline operator uint32_t() const { return (uint32_t) type; }

    inline bool operator== (const TriggerMode& other) const { return type == other.type; }
    inline bool operator== (const ID& id) const { return type == id; }
    inline bool operator== (const int& i) const { return (int) type == i; }
    inline bool operator!= (const TriggerMode& other) const { return type != other.type; }
    inline bool operator!= (const ID& id) const { return type != id; }
    inline bool operator!= (const int& i) const { return (int) type != i; }

    inline TriggerMode& operator= (const TriggerMode& other) {
        type = other.type;
        return *this;
    }
    inline TriggerMode& operator= (const TriggerMode::ID t) {
        type = t;
        return *this;
    }

private:
    ID type;
};

struct KeyInfo {
    int note;                  ///< Note number
    int length;                ///< Length as in 'Key Span'
    LayeringMode layeringMode; ///< The layering mode
    TriggerMode triggerMode;   ///< Trigger Mode
    int voiceGroup;            ///< Voice Group
    bool muted;                ///< Muted
    double volume;             ///< Volume (as in decibels)
    double gain;               ///< Gain as in SPL
    double fxGain[4];          ///< FX Gains
    double pitch;              ///< The overall pitch
    double randomPitch;        ///< Random Pitch Factor (unused)
    bool reverse;              ///< Play in reverse
    bool locked;               ///< Locked
    ADSR adsr;                 ///< ADSR for the note
};

} // namespace ksp1
