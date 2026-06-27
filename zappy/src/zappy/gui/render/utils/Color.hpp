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

    explicit Color(const ColorF& colorF) : _color{static_cast<::Color>(colorF.toColor())} {}

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
        std::seed_seq seed{static_cast<std::uint32_t>(teamIndex)};
        std::mt19937 rng(seed);
        const float hue = std::uniform_real_distribution<float>(0.0F, 360.0F)(rng);
        return ColorFromHSV(hue, 0.55F, 1.0F);
    }

    [[nodiscard]] Color operator+(const Color& other) const {
        return Color{toByte(static_cast<float>(_color.r) + static_cast<float>(other._color.r)),
                     toByte(static_cast<float>(_color.g) + static_cast<float>(other._color.g)),
                     toByte(static_cast<float>(_color.b) + static_cast<float>(other._color.b)),
                     toByte(static_cast<float>(_color.a) + static_cast<float>(other._color.a))};
    }

    [[nodiscard]] Color operator-(const Color& other) const {
        return Color{toByte(static_cast<float>(_color.r) - static_cast<float>(other._color.r)),
                     toByte(static_cast<float>(_color.g) - static_cast<float>(other._color.g)),
                     toByte(static_cast<float>(_color.b) - static_cast<float>(other._color.b)),
                     toByte(static_cast<float>(_color.a) - static_cast<float>(other._color.a))};
    }

    [[nodiscard]] Color operator*(float scalar) const {
        return Color{toByte(static_cast<float>(_color.r) * scalar), toByte(static_cast<float>(_color.g) * scalar),
                     toByte(static_cast<float>(_color.b) * scalar), toByte(static_cast<float>(_color.a) * scalar)};
    }

    [[nodiscard]] Color operator/(float scalar) const {
        if (scalar == 0.0F) {
            return *this;
        }
        return Color{toByte(static_cast<float>(_color.r) / scalar), toByte(static_cast<float>(_color.g) / scalar),
                     toByte(static_cast<float>(_color.b) / scalar), toByte(static_cast<float>(_color.a) / scalar)};
    }

    Color& operator+=(const Color& other) {
        _color.r = toByte(static_cast<float>(_color.r) + static_cast<float>(other._color.r));
        _color.g = toByte(static_cast<float>(_color.g) + static_cast<float>(other._color.g));
        _color.b = toByte(static_cast<float>(_color.b) + static_cast<float>(other._color.b));
        _color.a = toByte(static_cast<float>(_color.a) + static_cast<float>(other._color.a));
        return *this;
    }

    Color& operator-=(const Color& other) {
        _color.r = toByte(static_cast<float>(_color.r) - static_cast<float>(other._color.r));
        _color.g = toByte(static_cast<float>(_color.g) - static_cast<float>(other._color.g));
        _color.b = toByte(static_cast<float>(_color.b) - static_cast<float>(other._color.b));
        _color.a = toByte(static_cast<float>(_color.a) - static_cast<float>(other._color.a));
        return *this;
    }

    Color& operator*=(float scalar) {
        _color.r = toByte(static_cast<float>(_color.r) * scalar);
        _color.g = toByte(static_cast<float>(_color.g) * scalar);
        _color.b = toByte(static_cast<float>(_color.b) * scalar);
        _color.a = toByte(static_cast<float>(_color.a) * scalar);
        return *this;
    }

    Color& operator/=(float scalar) {
        if (scalar == 0.0F) {
            return *this;
        }
        _color.r = toByte(static_cast<float>(_color.r) / scalar);
        _color.g = toByte(static_cast<float>(_color.g) / scalar);
        _color.b = toByte(static_cast<float>(_color.b) / scalar);
        _color.a = toByte(static_cast<float>(_color.a) / scalar);
        return *this;
    }

    static constexpr ::Color kLIGHTGRAY{.r = static_cast<unsigned char>(200),
                                        .g = static_cast<unsigned char>(200),
                                        .b = static_cast<unsigned char>(200),
                                        .a = static_cast<unsigned char>(255)};  // Light Gray
    static constexpr ::Color kGRAY{.r = static_cast<unsigned char>(130),
                                   .g = static_cast<unsigned char>(130),
                                   .b = static_cast<unsigned char>(130),
                                   .a = static_cast<unsigned char>(255)};  // Gray
    static constexpr ::Color kDARKGRAY{.r = static_cast<unsigned char>(80),
                                       .g = static_cast<unsigned char>(80),
                                       .b = static_cast<unsigned char>(80),
                                       .a = static_cast<unsigned char>(255)};  // Dark Gray
    static constexpr ::Color kYELLOW{.r = static_cast<unsigned char>(253),
                                     .g = static_cast<unsigned char>(249),
                                     .b = static_cast<unsigned char>(0),
                                     .a = static_cast<unsigned char>(255)};  // Yellow
    static constexpr ::Color kGOLD{.r = static_cast<unsigned char>(255),
                                   .g = static_cast<unsigned char>(203),
                                   .b = static_cast<unsigned char>(0),
                                   .a = static_cast<unsigned char>(255)};  // Gold
    static constexpr ::Color kORANGE{.r = static_cast<unsigned char>(255),
                                     .g = static_cast<unsigned char>(161),
                                     .b = static_cast<unsigned char>(0),
                                     .a = static_cast<unsigned char>(255)};  // Orange
    static constexpr ::Color kPINK{.r = static_cast<unsigned char>(255),
                                   .g = static_cast<unsigned char>(109),
                                   .b = static_cast<unsigned char>(194),
                                   .a = static_cast<unsigned char>(255)};  // Pink
    static constexpr ::Color kRED{.r = static_cast<unsigned char>(230),
                                  .g = static_cast<unsigned char>(41),
                                  .b = static_cast<unsigned char>(55),
                                  .a = static_cast<unsigned char>(255)};  // Red
    static constexpr ::Color kMAROON{.r = static_cast<unsigned char>(190),
                                     .g = static_cast<unsigned char>(33),
                                     .b = static_cast<unsigned char>(55),
                                     .a = static_cast<unsigned char>(255)};  // Maroon
    static constexpr ::Color kGREEN{.r = static_cast<unsigned char>(0),
                                    .g = static_cast<unsigned char>(228),
                                    .b = static_cast<unsigned char>(48),
                                    .a = static_cast<unsigned char>(255)};  // Green
    static constexpr ::Color kLIME{.r = static_cast<unsigned char>(0),
                                   .g = static_cast<unsigned char>(158),
                                   .b = static_cast<unsigned char>(47),
                                   .a = static_cast<unsigned char>(255)};  // Lime
    static constexpr ::Color kDARKGREEN{.r = static_cast<unsigned char>(0),
                                        .g = static_cast<unsigned char>(117),
                                        .b = static_cast<unsigned char>(44),
                                        .a = static_cast<unsigned char>(255)};  // Dark Green
    static constexpr ::Color kSKYBLUE{.r = static_cast<unsigned char>(102),
                                      .g = static_cast<unsigned char>(191),
                                      .b = static_cast<unsigned char>(255),
                                      .a = static_cast<unsigned char>(255)};  // Sky Blue
    static constexpr ::Color kBLUE{.r = static_cast<unsigned char>(0),
                                   .g = static_cast<unsigned char>(121),
                                   .b = static_cast<unsigned char>(241),
                                   .a = static_cast<unsigned char>(255)};  // Blue
    static constexpr ::Color kDARKBLUE{.r = static_cast<unsigned char>(0),
                                       .g = static_cast<unsigned char>(82),
                                       .b = static_cast<unsigned char>(172),
                                       .a = static_cast<unsigned char>(255)};  // Dark Blue
    static constexpr ::Color kPURPLE{.r = static_cast<unsigned char>(200),
                                     .g = static_cast<unsigned char>(122),
                                     .b = static_cast<unsigned char>(255),
                                     .a = static_cast<unsigned char>(255)};  // Purple
    static constexpr ::Color kVIOLET{.r = static_cast<unsigned char>(135),
                                     .g = static_cast<unsigned char>(60),
                                     .b = static_cast<unsigned char>(190),
                                     .a = static_cast<unsigned char>(255)};  // Violet
    static constexpr ::Color kDARKPURPLE{.r = static_cast<unsigned char>(112),
                                         .g = static_cast<unsigned char>(31),
                                         .b = static_cast<unsigned char>(126),
                                         .a = static_cast<unsigned char>(255)};  // Dark Purple
    static constexpr ::Color kBEIGE{.r = static_cast<unsigned char>(211),
                                    .g = static_cast<unsigned char>(176),
                                    .b = static_cast<unsigned char>(131),
                                    .a = static_cast<unsigned char>(255)};  // Beige
    static constexpr ::Color kBROWN{.r = static_cast<unsigned char>(127),
                                    .g = static_cast<unsigned char>(106),
                                    .b = static_cast<unsigned char>(79),
                                    .a = static_cast<unsigned char>(255)};  // Brown
    static constexpr ::Color kDARKBROWN{.r = static_cast<unsigned char>(76),
                                        .g = static_cast<unsigned char>(63),
                                        .b = static_cast<unsigned char>(47),
                                        .a = static_cast<unsigned char>(255)};  // Dark Brown
    static constexpr ::Color kWHITE{.r = static_cast<unsigned char>(255),
                                    .g = static_cast<unsigned char>(255),
                                    .b = static_cast<unsigned char>(255),
                                    .a = static_cast<unsigned char>(255)};  // White
    static constexpr ::Color kBLACK{.r = static_cast<unsigned char>(0),
                                    .g = static_cast<unsigned char>(0),
                                    .b = static_cast<unsigned char>(0),
                                    .a = static_cast<unsigned char>(255)};  // Black
    static constexpr ::Color kBLANK{.r = static_cast<unsigned char>(0),
                                    .g = static_cast<unsigned char>(0),
                                    .b = static_cast<unsigned char>(0),
                                    .a = static_cast<unsigned char>(0)};  // Blank (Transparent)
    static constexpr ::Color kMAGENTA{.r = static_cast<unsigned char>(255),
                                      .g = static_cast<unsigned char>(0),
                                      .b = static_cast<unsigned char>(255),
                                      .a = static_cast<unsigned char>(255)};  // Magenta
    static constexpr ::Color kRAYWHITE{.r = static_cast<unsigned char>(245),
                                       .g = static_cast<unsigned char>(245),
                                       .b = static_cast<unsigned char>(245),
                                       .a = static_cast<unsigned char>(255)};  // My own White (raylib logo)
  private:
    static unsigned char toByte(float value) { return static_cast<unsigned char>(std::clamp(value, 0.0F, 255.0F)); }

    ::Color _color;
};

}  // namespace zappy::gui::render
