
#pragma once

namespace ksp1 {

template <typename T>
inline static T limit (T lower, T upper, T value) noexcept {
    return value < lower ? lower : (value > upper ? upper : value);
}

} // namespace ksp1
