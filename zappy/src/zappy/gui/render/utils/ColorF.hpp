/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** ColorF
*/

#pragma once
#include <raylib.h>

#include <algorithm>

namespace zappy::gui::render {
class Color;
class ColorF {
  public:
    ColorF() = default;
    ColorF(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
    explicit ColorF(const Color& c);
    explicit ColorF(const ::Color& c)
        : r(static_cast<float>(c.r)),
          g(static_cast<float>(c.g)),
          b(static_cast<float>(c.b)),
          a(static_cast<float>(c.a)) {}
    ColorF(const ColorF&) = default;
    ColorF& operator=(const ColorF&) = default;
    ColorF(ColorF&&) noexcept = default;
    ColorF& operator=(ColorF&&) noexcept = default;
    ~ColorF() = default;

    ColorF& operator+=(const ColorF& other) {
        r += other.r;
        g += other.g;
        b += other.b;
        a += other.a;
        return *this;
    }
    ColorF& operator-=(const ColorF& other) {
        r -= other.r;
        g -= other.g;
        b -= other.b;
        a -= other.a;
        return *this;
    }
    ColorF& operator*=(float scalar) {
        r *= scalar;
        g *= scalar;
        b *= scalar;
        a *= scalar;
        return *this;
    }
    ColorF& operator/=(float scalar) {
        r /= scalar;
        g /= scalar;
        b /= scalar;
        a /= scalar;
        return *this;
    }

    [[nodiscard]] ColorF operator+(const ColorF& other) const {
        return ColorF{r + other.red(), g + other.green(), b + other.blue(), a + other.alpha()};
    }
    [[nodiscard]] ColorF operator-(const ColorF& other) const {
        return ColorF{r - other.red(), g - other.green(), b - other.blue(), a - other.alpha()};
    }

    [[nodiscard]] ColorF operator*(float scalar) const {
        return ColorF{r * scalar, g * scalar, b * scalar, a * scalar};
    }

    [[nodiscard]] ColorF operator/(float scalar) const {
        if (scalar == 0.0F) {
            return *this;
        }
        return ColorF{r / scalar, g / scalar, b / scalar, a / scalar};
    }

    [[nodiscard]] float red() const { return r; }
    [[nodiscard]] float green() const { return g; }
    [[nodiscard]] float blue() const { return b; }
    [[nodiscard]] float alpha() const { return a; }

    [[nodiscard]] Color toColor() const;
    [[nodiscard]] explicit operator Color() const;

  private:
    ColorF FromColor(const Color& c);

    float r{0.0F};
    float g{0.0F};
    float b{0.0F};
    float a{0.0F};
};

}  // namespace zappy::gui::render
