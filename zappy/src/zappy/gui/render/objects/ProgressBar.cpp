/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** ProgressBar
*/

#include "ProgressBar.hpp"

#include <raylib.h>

#include <algorithm>

#include "zappy/gui/render/utils/Color.hpp"

namespace zappy::gui::render {

void ProgressBar::setPosition(int x, int y) {
    _x = x;
    _y = y;
}

void ProgressBar::setSize(int width, int height) {
    _width = width;
    _height = height;
}

void ProgressBar::setPadding(int padding) { _padding = padding; }

void ProgressBar::setColors(Color background, Color fill) {
    _background = background;
    _fill = fill;
}

void ProgressBar::update(float progress) { _progress = std::clamp(progress, 0.0F, 1.0F); }

void ProgressBar::draw() const {
    DrawRectangle(_x, _y, _width, _height, _background);

    const int track = _width - (2 * _padding);
    const int fillHeight = _height - (2 * _padding);
    const auto fillWidth = static_cast<int>(static_cast<float>(track) * _progress);

    DrawRectangle(_x + _padding, _y + _padding, fillWidth, fillHeight, _fill);
}

}  // namespace zappy::gui::render
