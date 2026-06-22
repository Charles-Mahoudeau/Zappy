/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Vector2
*/

#pragma once

#include <cstdint>

#include "helper.hpp"
#include "zappy/shared/exception/InvalidArgument.hpp"

namespace zappy::math {
template <IsArithmetic T>
struct Vector2 {
    T x{0};
    T y{0};

    [[nodiscard]] constexpr Vector2 wrapped(const Vector2& bounds) const noexcept;

    template <IsArithmetic U>
    constexpr explicit operator Vector2<U>() const noexcept;

    [[nodiscard]] constexpr auto operator<=>(const Vector2& other) const noexcept = default;

    constexpr Vector2& operator+=(const Vector2& other) noexcept;
    constexpr Vector2& operator-=(const Vector2& other) noexcept;
    constexpr Vector2& operator*=(const Vector2& other) noexcept;
    constexpr Vector2& operator/=(const Vector2& other);

    constexpr Vector2& operator+=(T scalar) noexcept;
    constexpr Vector2& operator-=(T scalar) noexcept;
    constexpr Vector2& operator*=(T scalar) noexcept;
    constexpr Vector2& operator/=(T scalar);

    [[nodiscard]] constexpr Vector2 operator+(const Vector2& other) const noexcept;
    [[nodiscard]] constexpr Vector2 operator-(const Vector2& other) const noexcept;
    [[nodiscard]] constexpr Vector2 operator*(const Vector2& other) const noexcept;
    [[nodiscard]] constexpr Vector2 operator/(const Vector2& other) const;

    [[nodiscard]] constexpr Vector2 operator+(T scalar) const noexcept;
    [[nodiscard]] constexpr Vector2 operator-(T scalar) const noexcept;
    [[nodiscard]] constexpr Vector2 operator*(T scalar) const noexcept;
    [[nodiscard]] constexpr Vector2 operator/(T scalar) const;
};

template <IsArithmetic T>
constexpr Vector2<T> Vector2<T>::wrapped(const Vector2& bounds) const noexcept {
    return {
        .x = mod(x, bounds.x),
        .y = mod(y, bounds.y),
    };
}

template <IsArithmetic T>
template <IsArithmetic U>
constexpr Vector2<T>::operator Vector2<U>() const noexcept {
    return {static_cast<U>(x), static_cast<U>(y)};
}

template <IsArithmetic T>
constexpr Vector2<T>& Vector2<T>::operator+=(const Vector2& other) noexcept {
    x += other.x;
    y += other.y;
    return *this;
}

template <IsArithmetic T>
constexpr Vector2<T>& Vector2<T>::operator-=(const Vector2& other) noexcept {
    x -= other.x;
    y -= other.y;
    return *this;
}

template <IsArithmetic T>
constexpr Vector2<T>& Vector2<T>::operator*=(const Vector2& other) noexcept {
    x *= other.x;
    y *= other.y;
    return *this;
}

template <IsArithmetic T>
constexpr Vector2<T>& Vector2<T>::operator/=(const Vector2& other) {
    if (other.x == 0 || other.y == 0) {
        throw exception::InvalidArgument{"division by zero"};
    }
    x /= other.x;
    y /= other.y;
    return *this;
}

template <IsArithmetic T>
constexpr Vector2<T>& Vector2<T>::operator+=(const T scalar) noexcept {
    x += scalar;
    y += scalar;
    return *this;
}

template <IsArithmetic T>
constexpr Vector2<T>& Vector2<T>::operator-=(const T scalar) noexcept {
    x -= scalar;
    y -= scalar;
    return *this;
}

template <IsArithmetic T>
constexpr Vector2<T>& Vector2<T>::operator*=(const T scalar) noexcept {
    x *= scalar;
    y *= scalar;
    return *this;
}

template <IsArithmetic T>
constexpr Vector2<T>& Vector2<T>::operator/=(const T scalar) {
    if (scalar == 0) {
        throw exception::InvalidArgument{"division by zero"};
    }
    x /= scalar;
    y /= scalar;
    return *this;
}

template <IsArithmetic T>
constexpr Vector2<T> Vector2<T>::operator+(const Vector2& other) const noexcept {
    Vector2 result = *this;

    result += other;
    return result;
}

template <IsArithmetic T>
constexpr Vector2<T> Vector2<T>::operator-(const Vector2& other) const noexcept {
    Vector2 result = *this;

    result -= other;
    return result;
}

template <IsArithmetic T>
constexpr Vector2<T> Vector2<T>::operator*(const Vector2& other) const noexcept {
    Vector2 result = *this;

    result *= other;
    return result;
}

template <IsArithmetic T>
constexpr Vector2<T> Vector2<T>::operator/(const Vector2& other) const {
    Vector2 result = *this;

    result /= other;
    return result;
}

template <IsArithmetic T>
constexpr Vector2<T> Vector2<T>::operator+(const T scalar) const noexcept {
    Vector2 result = *this;

    result += scalar;
    return result;
}

template <IsArithmetic T>
constexpr Vector2<T> Vector2<T>::operator-(const T scalar) const noexcept {
    Vector2 result = *this;

    result -= scalar;
    return result;
}

template <IsArithmetic T>
constexpr Vector2<T> Vector2<T>::operator*(const T scalar) const noexcept {
    Vector2 result = *this;

    result *= scalar;
    return result;
}

template <IsArithmetic T>
constexpr Vector2<T> Vector2<T>::operator/(T scalar) const {
    Vector2 result = *this;

    result /= scalar;
    return result;
}

template <IsArithmetic T>
[[nodiscard]] constexpr Vector2<T> operator+(T scalar, const Vector2<T>& vec) noexcept {
    Vector2<T> result = vec;

    result += scalar;
    return result;
}

template <IsArithmetic T>
[[nodiscard]] constexpr Vector2<T> operator-(T scalar, const Vector2<T>& vec) noexcept {
    Vector2<T> result = vec;

    result -= scalar;
    return result;
}

template <IsArithmetic T>
[[nodiscard]] constexpr Vector2<T> operator*(T scalar, const Vector2<T>& vec) noexcept {
    Vector2<T> result = vec;

    result *= scalar;
    return result;
}

template <IsArithmetic T>
[[nodiscard]] constexpr Vector2<T> operator/(T scalar, const Vector2<T>& vec) {
    Vector2<T> result = vec;

    result /= scalar;
    return result;
}

using Vector2i = Vector2<std::int32_t>;
using Vector2u = Vector2<std::uint32_t>;
using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;
}  // namespace zappy::math
