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

#ifndef KSP1_ARTICULATION_H
#define KSP1_ARTICULATION_H

#include "Types.h"

namespace KSP1 {

    class StrokeType
    {
    public:

        enum ID {
            SingleStroke = 0,
            DoubleStroke = 1,
            Roll         = 2
        };
        static const int32 numTypes = 3;

        inline StrokeType () : type (SingleStroke) { }
        inline StrokeType (const StrokeType::ID ot) : type (ot) { }
        inline StrokeType (const StrokeType& ot) : type (ot.type) { }
        inline StrokeType (const var& v)
        {
            if (v.isInt() || v.isInt64() || v.isDouble())
            {
                int32 t = (int32) v;
                if (isPositiveAndBelow (t, 3))
                    type = (ID) t;
                else
                    type = SingleStroke;

                return;
            }

            if (v.isString())
            {
                if (v.toString() == String ("singleStroke"))
                    type = SingleStroke;
                else if (v.isString() && v.toString() == String ("doubleStroke"))
                    type = DoubleStroke;
                else if (v.isString() && v.toString() == String ("roll"))
                    type = Roll;
                else { jassertfalse; }

                return;
            }

            jassertfalse;
            type = SingleStroke;
        }

        inline bool operator== (const StrokeType& o) const { return type == o.type; }
        inline bool operator== (const ID& o) const { return type == o; }
        inline bool operator== (const int32& o) const { return type == o; }
        inline bool operator!= (const StrokeType& o) const { return type != o.type; }
        inline bool operator!= (const ID& o) const { return type != o; }
        inline bool operator!= (const int32& o) const { return type != o; }

        inline String getName() const
        {
            switch (type) {
                case SingleStroke: return "Single Stroke"; break;
                case DoubleStroke: return "Double Stroke"; break;
                case Roll:         return "Roll"; break;
            }

            return "Unknown";

        }
        inline const ID& getType() const { return type; }
        inline operator int32()    const { return (int32) type; }

    private:
        ID type;
    };

    class Articulation :  public ObjectModel
    {
    public:

        Articulation()
            : ObjectModel (Tags::articulation)
        {
            setMissingProperties();
        }

        Articulation (const Articulation& o)
            : ObjectModel (o)
        {

        }

        Articulation (const ValueTree &data)
            : ObjectModel (data)
        {
            setMissingProperties();
        }

        inline String getTypeString() const
        {
            return getProperty (Slugs::type);
        }

        inline int32 getNoteNumber() const
        {
            ValueTree parent (node().getParent());
            return parent.getProperty (Slugs::note, -1);
        }

    private:
        void setMissingProperties();
    
    };
}

#endif
