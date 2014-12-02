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

#ifndef KSP1_MEDIA_OBJECT_H
#define KSP1_MEDIA_OBJECT_H

#include "KSP1.h"

namespace KSP1 {

    class MediaObject :  public MediaModel,
                         public ReferenceCountedObject
    {
    public:
        inline MediaObject () : MediaModel() { }
        inline MediaObject (const Identifier& type) : MediaModel (type) { }
        inline MediaObject (const ValueTree& data) : MediaModel (data) { }
        virtual ~MediaObject() { }

    private:

        friend class MediaManager;
    };

    typedef ReferenceCountedObjectPtr<MediaObject> MediaObjectPtr;

}

#endif // KSP1_MEDIA_OBJECT_H
