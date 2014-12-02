/*
    Articulator.h - This file is part of KSP1
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

#ifndef KSP1_ARTICULATOR_H
#define KSP1_ARTICULATOR_H

#include "Articulation.h"
#include "MidiFilter.h"

namespace KSP1 {

    class StrokeType;

    class Articulator :  public MidiFilter
    {
    public:

        Articulator();
        ~Articulator();

        bool mutate (const Articulation& art, const Identifier& prop);
        bool setStrokeType (int32 note, const StrokeType& type);

        void prepareToPlay (double sampleRate, int32 blockSize);
        void processMidi (MidiBuffer& midi, int32 nframes);
        void releaseResources();

    private:
        class Impl; Scoped<Impl> impl;
    };

}

#endif /* KSP1_ARTICULATOR_H */
