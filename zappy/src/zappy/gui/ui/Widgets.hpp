/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Widgets
*/

#pragma once

#include <string_view>

#include "zappy/gui/render/utils/Rectangle.hpp"
#include "zappy/gui/render/utils/Vector2.hpp"

namespace zappy::gui::ui {

class Widgets {
  public:
    Widgets() = delete;

    static constexpr float kPanelHeaderHeight = 24.0F;

    static void panel(render::Rectangle bounds, std::string_view title);
    static void label(render::Rectangle bounds, std::string_view text);
    static void centeredLabel(render::Rectangle bounds, std::string_view text);
    [[nodiscard]] static bool button(render::Rectangle bounds, std::string_view text);
    static render::Rectangle scrollPanel(render::Rectangle bounds, std::string_view title, render::Rectangle content,
                                         render::Vector2& scroll);
    static float slider(render::Rectangle bounds, std::string_view textLeft, std::string_view textRight, float current,
                        float min, float max);
    static int valueBox(render::Rectangle bounds, std::string_view text, int current, int min, int max, bool& editMode);
    static void progressBar(render::Rectangle bounds, std::string_view textLeft, std::string_view textRight,
                            float progress);
    static void beginScissor(render::Rectangle area);
    static void endScissor();
    static void overlay(render::Rectangle bounds);
};

}  // namespace zappy::gui::ui
