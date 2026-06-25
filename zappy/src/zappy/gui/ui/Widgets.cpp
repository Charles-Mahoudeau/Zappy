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
#include "zappy/gui/render/utils/Rectangle.hpp"
#include "zappy/gui/render/utils/Vector2.hpp"

namespace zappy::gui::ui {

void Widgets::panel(render::Rectangle bounds, std::string_view title) { GuiPanel(bounds, std::string{title}.c_str()); }

void Widgets::label(render::Rectangle bounds, std::string_view text) { GuiLabel(bounds, std::string{text}.c_str()); }

bool Widgets::button(render::Rectangle bounds, std::string_view text) {
    return GuiButton(bounds, std::string{text}.c_str()) != 0;
}

render::Rectangle Widgets::scrollPanel(render::Rectangle bounds, std::string_view title, render::Rectangle content,
                                       render::Vector2& scroll) {
    ::Rectangle view{};
    auto& nativeScroll = static_cast<::Vector2&>(scroll);
    GuiScrollPanel(bounds, std::string{title}.c_str(), content, &nativeScroll, &view);
    return render::Rectangle{view};
}

float Widgets::slider(render::Rectangle bounds, std::string_view textLeft, std::string_view textRight, float current,
                      float min, float max) {
    float value = current;
    GuiSlider(bounds, std::string{textLeft}.c_str(), std::string{textRight}.c_str(), &value, min, max);
    return value;
}

void Widgets::progressBar(render::Rectangle bounds, std::string_view textLeft, std::string_view textRight,
                          float progress) {
    float value = progress;
    GuiProgressBar(bounds, std::string{textLeft}.c_str(), std::string{textRight}.c_str(), &value, 0.0F, 1.0F);
}

void Widgets::beginScissor(render::Rectangle area) {
    BeginScissorMode(static_cast<int>(area.x()), static_cast<int>(area.y()), static_cast<int>(area.width()),
                     static_cast<int>(area.height()));
}

void Widgets::endScissor() { EndScissorMode(); }

void Widgets::overlay(render::Rectangle bounds) {
    static constexpr float kOverlayAlpha = 0.6F;
    DrawRectangle(static_cast<int>(bounds.x()), static_cast<int>(bounds.y()), static_cast<int>(bounds.width()),
                  static_cast<int>(bounds.height()), Fade(BLACK, kOverlayAlpha));
}

}  // namespace zappy::gui::ui
