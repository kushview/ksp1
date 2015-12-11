/*
    LayeringMode.h - This file is part of KSP1
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
#ifndef KSP1_LAYERING_MODE_H
#define KSP1_LAYERING_MODE_H

#include "KSP1.h"

namespace KSP1 {

class LayeringMode
{
public:
    enum ID
    {
        Combine = 0,    ///< Layers are mixed (combined)
        RoundRobin,     ///< Layers are alternated upon each note-on
        VelocityRange   ///< Layer is selected according to a velocity range
    };

    LayeringMode() : mode (Combine) { }

    LayeringMode (const int32& m)
    {
        assert (m < VelocityRange + 1);
        mode = static_cast<ID> (m);
    }

    LayeringMode (const ID& id) : mode (id) { }

    LayeringMode (const String& id)
    {
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
    bool operator== (const int32& o) const { return o == mode; }
    bool operator!= (const LayeringMode& o) const { return o.mode != mode; }
    bool operator!= (const int32& o) const { return o != mode; }

    LayeringMode& operator= (const LayeringMode& o)
    {
        mode = o.mode;
        return *this;
    }

private:
    int32 mode;
};

}

#endif // KSP1_LAYERING_MODE_H
