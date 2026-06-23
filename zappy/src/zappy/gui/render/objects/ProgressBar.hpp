/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** ProgressBar
*/

#pragma once

#include "zappy/gui/render/utils/Color.hpp"

namespace zappy::gui::render {
class ProgressBar {
  public:
    ProgressBar() = default;

    void setPosition(int x, int y);
    void setSize(int width, int height);
    void setPadding(int padding);
    void setColors(Color background, Color fill);

    void update(float progress);

    void draw() const;

  private:
    static constexpr int kDefaultPadding = 4;

    int _x{0};
    int _y{0};
    int _width{0};
    int _height{0};
    int _padding{kDefaultPadding};
    float _progress{0.0F};
    Color _background{Color::kWHITE};
    Color _fill{Color::kGRAY};
};

}  // namespace zappy::gui::render
