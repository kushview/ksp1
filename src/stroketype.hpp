#pragma once

#include <string>

namespace ksp1 {

class StrokeType {
public:
    enum ID {
        SingleStroke = 0,
        DoubleStroke = 1,
        Roll         = 2
    };

    static constexpr auto numTypes = 3;

    inline StrokeType() noexcept : type (SingleStroke) {}
    inline StrokeType (const StrokeType::ID ot) noexcept : type (ot) {}
    inline StrokeType (const StrokeType& ot) noexcept : type (ot.type) {}

    inline bool operator== (const StrokeType& o) const noexcept { return type == o.type; }
    inline bool operator== (const ID& o) const noexcept { return type == o; }
    inline bool operator== (const int& o) const noexcept { return type == o; }
    inline bool operator!= (const StrokeType& o) const noexcept { return type != o.type; }
    inline bool operator!= (const ID& o) const noexcept { return type != o; }
    inline bool operator!= (const int& o) const noexcept { return type != o; }

    inline std::string str() const noexcept {
        switch (type) {
            case SingleStroke:
                return "Single Stroke";
                break;
            case DoubleStroke:
                return "Double Stroke";
                break;
            case Roll:
                return "Roll";
                break;
        }

        return "Unknown";
    }

    inline ID kind() const noexcept { return type; }
    inline operator int() const noexcept { return static_cast<int> (type); }

private:
    ID type;
};

} // namespace ksp1
