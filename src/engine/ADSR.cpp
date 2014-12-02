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

#include "engine/ADSR.h"

namespace KSP1 {

    ADSR::ADSR (float a, float d, float s, float r)
        : adsrState (Attack),
          adsrValue (0.0f),
          futureRelease (0.0f),
          modeTicks (0.0f),
          totalTicks (0.0f)
    {
       setAttack (a);
       setDecay (d);
       setSustain (s);
       setRelease (r);
    }


    ADSR::ADSR (const ADSR& other)
    {
        copy (other);
    }

    ADSR::~ADSR() { }

    ADSR&
    ADSR::operator= (const ADSR& other)
    {
        copy (other);
        return *this;
    }

    void
    ADSR::copy (const ADSR& other)
    {
        adsrState  = other.adsrState;
        adsrValue  = other.adsrValue;
        futureRelease = other.futureRelease;
        modeTicks  = other.modeTicks;
        totalTicks = other.totalTicks;

        sync (other, false);
    }

    void
    ADSR::sync (const ADSR &other, bool resetTicks)
    {
        if (resetTicks)
            reset();

        setAttack  (other.attackTime);
        setDecay   (other.decayTime);
        setSustain (other.sustainLevel);
        setRelease (other.releaseTime);
    }

}
