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

#include "Articulation.h"

namespace KSP1 {
   
    void Articulation::setMissingProperties()
    {
        ValueTree n (node());
        
        if (! n.hasProperty (Tags::type))
            n.setProperty (Tags::type, "singleStroke", nullptr);
        if (! n.hasProperty (Tags::interval))
            n.setProperty (Tags::interval, (double)0.25, nullptr);
        if (! n.hasProperty (Tags::legato))
            n.setProperty (Tags::legato, (double)0.6, nullptr);

    }
}
