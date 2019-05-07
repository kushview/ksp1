#pragma once

namespace KSP1 {

class StrokeType
{
public:
    enum ID {
        SingleStroke = 0,
        DoubleStroke = 1,
        Roll         = 2
    };
    static const int numTypes = 3;

    inline StrokeType () : type (SingleStroke) { }
    inline StrokeType (const StrokeType::ID ot) : type (ot) { }
    inline StrokeType (const StrokeType& ot) : type (ot.type) { }
    inline StrokeType (const var& v)
    {
        if (v.isInt() || v.isInt64() || v.isDouble())
        {
            int t = (int) v;
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
    inline bool operator== (const int& o) const { return type == o; }
    inline bool operator!= (const StrokeType& o) const { return type != o.type; }
    inline bool operator!= (const ID& o) const { return type != o; }
    inline bool operator!= (const int& o) const { return type != o; }

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
    inline operator int()    const { return (int) type; }

private:
    ID type;
};

}
