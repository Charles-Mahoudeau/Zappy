/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Widgets
*/

#include "Widgets.hpp"

#include <string>
#include <string_view>

#include "raygui.h"
#include "raylib.h"
#include "utils/Rectangle.hpp"
#include "utils/Vector2.hpp"

namespace zappy::gui::ui {

void Widgets::panel(Rectangle bounds, std::string_view title) { GuiPanel(bounds, std::string{title}.c_str()); }

void Widgets::label(Rectangle bounds, std::string_view text) { GuiLabel(bounds, std::string{text}.c_str()); }

Rectangle Widgets::scrollPanel(Rectangle bounds, std::string_view title, Rectangle content, Vector2& scroll) {
    ::Rectangle view{};
    auto& nativeScroll = static_cast<::Vector2&>(scroll);
    GuiScrollPanel(bounds, std::string{title}.c_str(), content, &nativeScroll, &view);
    return Rectangle{view};
}

float Widgets::slider(Rectangle bounds, std::string_view textLeft, std::string_view textRight, float current, float min,
                      float max) {
    float value = current;
    GuiSlider(bounds, std::string{textLeft}.c_str(), std::string{textRight}.c_str(), &value, min, max);
    return value;
}

}  // namespace zappy::gui::ui
