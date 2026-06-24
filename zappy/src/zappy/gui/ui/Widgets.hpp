/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Widgets
*/

#pragma once

#include <string_view>

#include "utils/Rectangle.hpp"
#include "utils/Vector2.hpp"

namespace zappy::gui::ui {

class Widgets {
  public:
    Widgets() = delete;

    static constexpr float kPanelHeaderHeight = 24.0F;

    static void panel(Rectangle bounds, std::string_view title);
    static void label(Rectangle bounds, std::string_view text);
    [[nodiscard]] static bool button(Rectangle bounds, std::string_view text);
    static Rectangle scrollPanel(Rectangle bounds, std::string_view title, Rectangle content, Vector2& scroll);
    static float slider(Rectangle bounds, std::string_view textLeft, std::string_view textRight, float current,
                        float min, float max);
    static void progressBar(Rectangle bounds, std::string_view textLeft, std::string_view textRight, float progress);
    static void beginScissor(Rectangle area);
    static void endScissor();
    static void overlay(Rectangle bounds);
};

}  // namespace zappy::gui::ui
