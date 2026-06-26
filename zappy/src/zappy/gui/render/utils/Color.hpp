/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Color
*/

#pragma once

#include <raylib.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <random>

#include "ColorF.hpp"

namespace zappy::gui::render {
class Color {
  public:
    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
        : _color{.r = r, .g = g, .b = b, .a = a} {}
    explicit Color(const ::Color& color) : _color{color} {}
    Color(const Color&) = default;
    Color& operator=(const Color&) = default;
    Color(Color&&) noexcept = default;
    Color& operator=(Color&&) noexcept = default;
    ~Color() = default;

    Color(const ColorF& colorF) : _color{static_cast<::Color>(colorF.toColor())} {}

    [[nodiscard]] ::Color get() const { return _color; }

    operator ::Color() const { return _color; }
    operator ::Color&() { return _color; }

    Color& operator=(const ::Color& color) {
        _color = color;
        return *this;
    }

    [[nodiscard]] static ::Color teamTint(std::size_t teamIndex, std::size_t modelCount) {
        if (modelCount == 0 || teamIndex < modelCount) {
            return kWHITE;
        }
        std::mt19937 rng(static_cast<std::uint32_t>(teamIndex));
        const float hue = std::uniform_real_distribution<float>(0.0F, 360.0F)(rng);
        return ColorFromHSV(hue, 0.55F, 1.0F);
    }

    [[nodiscard]] Color operator+(const Color& other) const {
        return Color{static_cast<unsigned char>(std::min(255, _color.r + other._color.r)),
                     static_cast<unsigned char>(std::min(255, _color.g + other._color.g)),
                     static_cast<unsigned char>(std::min(255, _color.b + other._color.b)),
                     static_cast<unsigned char>(std::min(255, _color.a + other._color.a))};
    }

    [[nodiscard]] Color operator-(const Color& other) const {
        return Color{static_cast<unsigned char>(std::max(0, _color.r - other._color.r)),
                     static_cast<unsigned char>(std::max(0, _color.g - other._color.g)),
                     static_cast<unsigned char>(std::max(0, _color.b - other._color.b)),
                     static_cast<unsigned char>(std::max(0, _color.a - other._color.a))};
    }

    [[nodiscard]] Color operator*(float scalar) const {
        return Color{static_cast<unsigned char>(std::clamp(static_cast<int>(_color.r * scalar), 0, 255)),
                     static_cast<unsigned char>(std::clamp(static_cast<int>(_color.g * scalar), 0, 255)),
                     static_cast<unsigned char>(std::clamp(static_cast<int>(_color.b * scalar), 0, 255)),
                     static_cast<unsigned char>(std::clamp(static_cast<int>(_color.a * scalar), 0, 255))};
    }

    [[nodiscard]] Color operator/(float scalar) const {
        if (scalar == 0.0F) {
            return *this;
        }
        return Color{static_cast<unsigned char>(std::clamp(static_cast<int>(_color.r / scalar), 0, 255)),
                     static_cast<unsigned char>(std::clamp(static_cast<int>(_color.g / scalar), 0, 255)),
                     static_cast<unsigned char>(std::clamp(static_cast<int>(_color.b / scalar), 0, 255)),
                     static_cast<unsigned char>(std::clamp(static_cast<int>(_color.a / scalar), 0, 255))};
    }

    Color& operator+=(const Color& other) {
        _color.r = static_cast<unsigned char>(std::min(255, _color.r + other._color.r));
        _color.g = static_cast<unsigned char>(std::min(255, _color.g + other._color.g));
        _color.b = static_cast<unsigned char>(std::min(255, _color.b + other._color.b));
        _color.a = static_cast<unsigned char>(std::min(255, _color.a + other._color.a));
        return *this;
    }

    Color& operator-=(const Color& other) {
        _color.r = static_cast<unsigned char>(std::max(0, _color.r - other._color.r));
        _color.g = static_cast<unsigned char>(std::max(0, _color.g - other._color.g));
        _color.b = static_cast<unsigned char>(std::max(0, _color.b - other._color.b));
        _color.a = static_cast<unsigned char>(std::max(0, _color.a - other._color.a));
        return *this;
    }

    Color& operator*=(float scalar) {
        _color.r = static_cast<unsigned char>(std::clamp(static_cast<int>(_color.r * scalar), 0, 255));
        _color.g = static_cast<unsigned char>(std::clamp(static_cast<int>(_color.g * scalar), 0, 255));
        _color.b = static_cast<unsigned char>(std::clamp(static_cast<int>(_color.b * scalar), 0, 255));
        _color.a = static_cast<unsigned char>(std::clamp(static_cast<int>(_color.a * scalar), 0, 255));
        return *this;
    }

    Color& operator/=(float scalar) {
        if (scalar == 0.0F) {
            return *this;
        }
        _color.r = static_cast<unsigned char>(std::clamp(static_cast<int>(_color.r / scalar), 0, 255));
        _color.g = static_cast<unsigned char>(std::clamp(static_cast<int>(_color.g / scalar), 0, 255));
        _color.b = static_cast<unsigned char>(std::clamp(static_cast<int>(_color.b / scalar), 0, 255));
        _color.a = static_cast<unsigned char>(std::clamp(static_cast<int>(_color.a / scalar), 0, 255));
        return *this;
    }

    static constexpr ::Color kLIGHTGRAY{.r = 200, .g = 200, .b = 200, .a = 255};  // Light Gray
    static constexpr ::Color kGRAY{.r = 130, .g = 130, .b = 130, .a = 255};       // Gray
    static constexpr ::Color kDARKGRAY{.r = 80, .g = 80, .b = 80, .a = 255};      // Dark Gray
    static constexpr ::Color kYELLOW{.r = 253, .g = 249, .b = 0, .a = 255};       // Yellow
    static constexpr ::Color kGOLD{.r = 255, .g = 203, .b = 0, .a = 255};         // Gold
    static constexpr ::Color kORANGE{.r = 255, .g = 161, .b = 0, .a = 255};       // Orange
    static constexpr ::Color kPINK{.r = 255, .g = 109, .b = 194, .a = 255};       // Pink
    static constexpr ::Color kRED{.r = 230, .g = 41, .b = 55, .a = 255};          // Red
    static constexpr ::Color kMAROON{.r = 190, .g = 33, .b = 55, .a = 255};       // Maroon
    static constexpr ::Color kGREEN{.r = 0, .g = 228, .b = 48, .a = 255};         // Green
    static constexpr ::Color kLIME{.r = 0, .g = 158, .b = 47, .a = 255};          // Lime
    static constexpr ::Color kDARKGREEN{.r = 0, .g = 117, .b = 44, .a = 255};     // Dark Green
    static constexpr ::Color kSKYBLUE{.r = 102, .g = 191, .b = 255, .a = 255};    // Sky Blue
    static constexpr ::Color kBLUE{.r = 0, .g = 121, .b = 241, .a = 255};         // Blue
    static constexpr ::Color kDARKBLUE{.r = 0, .g = 82, .b = 172, .a = 255};      // Dark Blue
    static constexpr ::Color kPURPLE{.r = 200, .g = 122, .b = 255, .a = 255};     // Purple
    static constexpr ::Color kVIOLET{.r = 135, .g = 60, .b = 190, .a = 255};      // Violet
    static constexpr ::Color kDARKPURPLE{.r = 112, .g = 31, .b = 126, .a = 255};  // Dark Purple
    static constexpr ::Color kBEIGE{.r = 211, .g = 176, .b = 131, .a = 255};      // Beige
    static constexpr ::Color kBROWN{.r = 127, .g = 106, .b = 79, .a = 255};       // Brown
    static constexpr ::Color kDARKBROWN{.r = 76, .g = 63, .b = 47, .a = 255};     // Dark Brown
    static constexpr ::Color kWHITE{.r = 255, .g = 255, .b = 255, .a = 255};      // White
    static constexpr ::Color kBLACK{.r = 0, .g = 0, .b = 0, .a = 255};            // Black
    static constexpr ::Color kBLANK{.r = 0, .g = 0, .b = 0, .a = 0};              // Blank (Transparent)
    static constexpr ::Color kMAGENTA{.r = 255, .g = 0, .b = 255, .a = 255};      // Magenta
    static constexpr ::Color kRAYWHITE{.r = 245, .g = 245, .b = 245, .a = 255};   // My own White (raylib logo)
  private:
    ::Color _color;
};

}  // namespace zappy::gui::render
