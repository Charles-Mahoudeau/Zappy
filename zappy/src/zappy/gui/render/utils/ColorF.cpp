/*
** EPITECH PROJECT, 2026
** projectMirror
** File description:
** ColorF
*/

#include "ColorF.hpp"

#include "Color.hpp"

namespace zappy::gui::render {

ColorF::ColorF(const Color& c) : ColorF(FromColor(c)) {}

ColorF ColorF::FromColor(const Color& c) {
    auto rayColor = c.get();
    return ColorF{static_cast<float>(rayColor.r), static_cast<float>(rayColor.g), static_cast<float>(rayColor.b),
                  static_cast<float>(rayColor.a)};
}

Color ColorF::toColor() const {
    return Color{static_cast<unsigned char>(std::clamp(r, 0.0F, 255.0F)),
                 static_cast<unsigned char>(std::clamp(g, 0.0F, 255.0F)),
                 static_cast<unsigned char>(std::clamp(b, 0.0F, 255.0F)),
                 static_cast<unsigned char>(std::clamp(a, 0.0F, 255.0F))};
}

ColorF::operator Color() const { return toColor(); }

}  // namespace zappy::gui::render
