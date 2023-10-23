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

#ifndef PATTERNLOADER_H
#define PATTERNLOADER_H

#include "../sampler/Pattern.h"
#include "InstrumentLoader.h"

namespace KSP1 {

class PatternLoader : public AssetLoader {
    const KnownFilesystems& fses;

public:
    PatternLoader (Pattern& p, const KnownFilesystems& f)
        : fses (f),
          patsy (p) {
    }

protected:
    Pattern& patsy;
    OwnedArray<MidiMessageSequence> midi;
};

} // namespace KSP1
#endif // PATTERNLOADER_H
