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

#include "lowpass.hpp"

namespace ksp1 {

LowPassFilter::LowPassFilter()
    : cutoff (1.0f),
      resonance (0.0f) {
    resetBuffers();
}

LowPassFilter::LowPassFilter (const LowPassFilter& lpf)
    : cutoff (lpf.cutoff), resonance (lpf.resonance) {
    resetBuffers();
}

void LowPassFilter::resetBuffers() {
    bandPassBuffer_L = 0.0f;
    bandPassBuffer_R = 0.0f;
    lowPassBuffer_L  = 0.0f;
    lowPassBuffer_R  = 0.0f;
}

} // namespace ksp1
