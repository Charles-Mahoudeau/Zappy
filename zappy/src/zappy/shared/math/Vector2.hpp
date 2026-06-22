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

    constexpr Vector2() noexcept = default;
    constexpr Vector2(T x, T y) noexcept;
    constexpr explicit Vector2(T x) noexcept;

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

    friend constexpr Vector2 operator+(Vector2 lhs, const Vector2& rhs) noexcept {
        lhs += rhs;
        return lhs;
    }
    friend constexpr Vector2 operator-(Vector2 lhs, const Vector2& rhs) noexcept {
        lhs -= rhs;
        return lhs;
    }
    friend constexpr Vector2 operator*(Vector2 lhs, const Vector2& rhs) noexcept {
        lhs *= rhs;
        return lhs;
    }
    friend constexpr Vector2 operator/(Vector2 lhs, const Vector2& rhs) {
        lhs /= rhs;
        return lhs;
    }

    friend constexpr Vector2 operator+(Vector2 lhs, const T scalar) noexcept {
        lhs += scalar;
        return lhs;
    }
    friend constexpr Vector2 operator-(Vector2 lhs, const T scalar) noexcept {
        lhs -= scalar;
        return lhs;
    }
    friend constexpr Vector2 operator*(Vector2 lhs, const T scalar) noexcept {
        lhs *= scalar;
        return lhs;
    }
    friend constexpr Vector2 operator/(Vector2 lhs, const T scalar) {
        lhs /= scalar;
        return lhs;
    }

    friend constexpr Vector2 operator+(const T scalar, Vector2 rhs) noexcept {
        rhs += scalar;
        return rhs;
    }
    friend constexpr Vector2 operator-(const T scalar, Vector2 rhs) noexcept {
        return {scalar - rhs.x, scalar - rhs.y};
    }
    friend constexpr Vector2 operator*(const T scalar, Vector2 rhs) noexcept {
        rhs *= scalar;
        return rhs;
    }
    friend constexpr Vector2 operator/(const T scalar, Vector2 rhs) {
        if (rhs.x == 0 || rhs.y == 0) {
            throw exception::InvalidArgument{"division by zero"};
        }
        return {scalar / rhs.x, scalar / rhs.y};
    }
};

template <IsArithmetic T>
constexpr Vector2<T>::Vector2(T x, T y) noexcept : x{x}, y{y} {}

template <IsArithmetic T>
constexpr Vector2<T>::Vector2(T x) noexcept : x{x}, y{x} {}

template <IsArithmetic T>
constexpr Vector2<T> Vector2<T>::wrapped(const Vector2& bounds) const noexcept {
    return {
        mod(x, bounds.x),
        mod(y, bounds.y),
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

using Vector2i = Vector2<std::int32_t>;
using Vector2u = Vector2<std::uint32_t>;
using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;
}  // namespace zappy::math
