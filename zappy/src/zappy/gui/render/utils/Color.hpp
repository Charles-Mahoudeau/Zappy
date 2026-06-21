/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Color
*/

#pragma once

#include <raylib.h>

#undef LIGHTGRAY
#undef GRAY
#undef DARKGRAY
#undef YELLOW
#undef GOLD
#undef ORANGE
#undef PINK
#undef RED
#undef MAROON
#undef GREEN
#undef LIME
#undef DARKGREEN
#undef SKYBLUE
#undef BLUE
#undef DARKBLUE
#undef PURPLE
#undef VIOLET
#undef DARKPURPLE
#undef BEIGE
#undef BROWN
#undef DARKBROWN
#undef WHITE
#undef BLACK
#undef BLANK
#undef MAGENTA
#undef RAYWHITE

namespace zappy::gui::render {
class Color {
  public:
    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
        : _color{.r = r, .g = g, .b = b, .a = a} {}
    Color(const ::Color& color) : _color{color} {}
    Color(const Color&) = default;
    Color& operator=(const Color&) = default;
    Color(Color&&) noexcept = default;
    Color& operator=(Color&&) noexcept = default;
    ~Color() = default;

    [[nodiscard]] ::Color get() const { return _color; }

    operator ::Color() const { return _color; }
    operator ::Color&() { return _color; }

    static constexpr ::Color LIGHTGRAY{.r = 200, .g = 200, .b = 200, .a = 255};  // Light Gray
    static constexpr ::Color GRAY{.r = 130, .g = 130, .b = 130, .a = 255};       // Gray
    static constexpr ::Color DARKGRAY{.r = 80, .g = 80, .b = 80, .a = 255};      // Dark Gray
    static constexpr ::Color YELLOW{.r = 253, .g = 249, .b = 0, .a = 255};       // Yellow
    static constexpr ::Color GOLD{.r = 255, .g = 203, .b = 0, .a = 255};         // Gold
    static constexpr ::Color ORANGE{.r = 255, .g = 161, .b = 0, .a = 255};       // Orange
    static constexpr ::Color PINK{.r = 255, .g = 109, .b = 194, .a = 255};       // Pink
    static constexpr ::Color RED{.r = 230, .g = 41, .b = 55, .a = 255};          // Red
    static constexpr ::Color MAROON{.r = 190, .g = 33, .b = 55, .a = 255};       // Maroon
    static constexpr ::Color GREEN{.r = 0, .g = 228, .b = 48, .a = 255};         // Green
    static constexpr ::Color LIME{.r = 0, .g = 158, .b = 47, .a = 255};          // Lime
    static constexpr ::Color DARKGREEN{.r = 0, .g = 117, .b = 44, .a = 255};     // Dark Green
    static constexpr ::Color SKYBLUE{.r = 102, .g = 191, .b = 255, .a = 255};    // Sky Blue
    static constexpr ::Color BLUE{.r = 0, .g = 121, .b = 241, .a = 255};         // Blue
    static constexpr ::Color DARKBLUE{.r = 0, .g = 82, .b = 172, .a = 255};      // Dark Blue
    static constexpr ::Color PURPLE{.r = 200, .g = 122, .b = 255, .a = 255};     // Purple
    static constexpr ::Color VIOLET{.r = 135, .g = 60, .b = 190, .a = 255};      // Violet
    static constexpr ::Color DARKPURPLE{.r = 112, .g = 31, .b = 126, .a = 255};  // Dark Purple
    static constexpr ::Color BEIGE{.r = 211, .g = 176, .b = 131, .a = 255};      // Beige
    static constexpr ::Color BROWN{.r = 127, .g = 106, .b = 79, .a = 255};       // Brown
    static constexpr ::Color DARKBROWN{.r = 76, .g = 63, .b = 47, .a = 255};     // Dark Brown
    static constexpr ::Color WHITE{.r = 255, .g = 255, .b = 255, .a = 255};      // White
    static constexpr ::Color BLACK{.r = 0, .g = 0, .b = 0, .a = 255};            // Black
    static constexpr ::Color BLANK{.r = 0, .g = 0, .b = 0, .a = 0};              // Blank (Transparent)
    static constexpr ::Color MAGENTA{.r = 255, .g = 0, .b = 255, .a = 255};      // Magenta
    static constexpr ::Color RAYWHITE{.r = 245, .g = 245, .b = 245, .a = 255};   // My own White (raylib logo)
  private:
    ::Color _color;
};

}  // namespace zappy::gui::render
