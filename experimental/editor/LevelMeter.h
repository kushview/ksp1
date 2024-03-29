/*
    This file is part of KSP1
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

#pragma once

#include "KSP1.h"

namespace KSP1 {

class LevelMeter : public kv::DigitalMeter {
public:
    LevelMeter()
        : DigitalMeter (2) {}

    LevelMeter (int ports, bool horiz)
        : DigitalMeter (jmax (1, ports), horiz) {}

    virtual ~LevelMeter() {}
};

} // namespace KSP1
