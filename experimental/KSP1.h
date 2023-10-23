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

#include "JuceHeader.h"
#include <boost/signals2.hpp>

namespace KSP1 {

inline static int generateObjectID (int salt = 0) {
    Random r (salt == 0 ? Time::currentTimeMillis() : salt);

    static int lastGenerated = 0;
    if (lastGenerated == 0) {
        lastGenerated = r.nextInt (Range<int> (1, std::numeric_limits<int>::max()));
        return lastGenerated;
    }

    int newId = r.nextInt (Range<int> (1, std::numeric_limits<int>::max()));
    while (newId == lastGenerated) {
        newId = r.nextInt (Range<int> (1, std::numeric_limits<int>::max()));
    }
    lastGenerated = newId;
    return newId;
}

typedef kv::AtomicValue<int> AtomicInt;
typedef kv::AtomicLock DataLock;
typedef kv::AtomicValue<double> AtomicDouble;
typedef kv::AtomicValue<bool> AtomicFlag;
typedef kv::AtomicValue<float> AtomicFloat;
typedef kv::AtomicValue<int64> AtomicFrame;

} // namespace KSP1

namespace Tags {

using namespace kv::Slugs;
static const Identifier articulation  = "articulation";
static const Identifier interval      = "interval";
static const Identifier instrument    = "instrument";
static const Identifier key           = "key";
static const Identifier layer         = "layer";
static const Identifier legato        = "legato";
static const Identifier object        = "object";
static const Identifier panning       = "panning";
static const Identifier session       = "session";
static const Identifier strokeType    = "strokeType";
static const Identifier triggerMode   = "triggerMode";
static const Identifier voiceGroup    = "voiceGroup";
static const Identifier velocityUpper = "velocityUpper";
static const Identifier velocityLower = "velocityLower";
static const Identifier resonance     = "resonance";
static const Identifier cutoff        = "cutoff";

} // namespace Tags
