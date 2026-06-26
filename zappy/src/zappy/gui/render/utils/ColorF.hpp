/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** ColorF
*/

#pragma once
#include <algorithm>

#include "Color.hpp"

namespace zappy::gui::render {

class ColorF {
  public:
    ColorF() = default;
    ColorF(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
    explicit ColorF(const Color& c) {
        auto rayColor = c.get();
        r = static_cast<float>(rayColor.r);
        g = static_cast<float>(rayColor.g);
        b = static_cast<float>(rayColor.b);
        a = static_cast<float>(rayColor.a);
    }

    ColorF& operator+=(const ColorF& other) {
        r += other.r;
        g += other.g;
        b += other.b;
        a += other.a;
        return *this;
    }
    ColorF operator+(const ColorF& other) const { return ColorF{r + other.r, g + other.g, b + other.b, a + other.a}; }

    ColorF& operator-=(const ColorF& other) {
        r -= other.r;
        g -= other.g;
        b -= other.b;
        a -= other.a;
        return *this;
    }
    ColorF operator-(const ColorF& other) const { return ColorF{r - other.r, g - other.g, b - other.b, a - other.a}; }

    ColorF operator*(float scalar) const { return {r * scalar, g * scalar, b * scalar, a * scalar}; }

    ColorF& operator*=(float scalar) {
        r *= scalar;
        g *= scalar;
        b *= scalar;
        a *= scalar;
        return *this;
    }

    ColorF operator/(float scalar) const { return {r / scalar, g / scalar, b / scalar, a / scalar}; }
    ColorF& operator/=(float scalar) {
        r /= scalar;
        g /= scalar;
        b /= scalar;
        a /= scalar;
        return *this;
    }

    [[nodiscard]] Color toColor() const {
        return Color{static_cast<unsigned char>(std::clamp(r, 0.0F, 255.0F)),
                     static_cast<unsigned char>(std::clamp(g, 0.0F, 255.0F)),
                     static_cast<unsigned char>(std::clamp(b, 0.0F, 255.0F)),
                     static_cast<unsigned char>(std::clamp(a, 0.0F, 255.0F))};
    }

    [[nodiscard]] operator Color() const { return toColor(); }

  private:
    float r{0.0F};
    float g{0.0F};
    float b{0.0F};
    float a{0.0F};
};

// Allows float * ColorF
inline ColorF operator*(float scalar, const ColorF& color) { return color * scalar; }

}  // namespace zappy::gui::render
