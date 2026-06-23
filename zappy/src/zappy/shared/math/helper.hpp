/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** helper
*/

#pragma once

#include <cmath>
#include <concepts>
#include <type_traits>

namespace zappy::math {
template <typename T>
concept IsArithmetic = std::is_arithmetic_v<T>;

/// @brief Calculates the true mathematical floored modulo, safely wrapping negative numbers.
/// @tparam T An arithmetic type (integral or floating-point) satisfying the IsArithmetic concept.
/// @param x The dividend value to wrap.
/// @param n The divisor representing the modulo base (e.g., map width or height).
/// @return The wrapped positive remainder within the range [0, n).
template <IsArithmetic T>
[[nodiscard]] constexpr T mod(T x, T n) noexcept(std::is_integral_v<T>) {
    if constexpr (std::integral<T>) {
        T remainder = x % n;

        return remainder >= 0 ? remainder : remainder + n;
    }

    T remainder = std::fmod(x, n);

    return remainder >= 0 ? remainder : remainder + n;
}
}  // namespace zappy::math
