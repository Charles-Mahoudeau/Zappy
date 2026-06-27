/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** DotAnimator
*/

#include "DotAnimator.hpp"

#include <cstddef>
#include <format>
#include <string>

namespace zappy::gui::ui {

DotAnimator::DotAnimator(int maxDots) : _maxDots{maxDots} {}

void DotAnimator::advance() { _dots = (_dots % _maxDots) + 1; }

void DotAnimator::update(float dt) {
    _timer += dt;
    if (_timer >= kIntervalSeconds) {
        _timer -= kIntervalSeconds;
        advance();
    }
}

std::string DotAnimator::text(std::string_view label) const {
    return std::format("{}{}", label, std::string(static_cast<std::size_t>(_dots), '.'));
}

}  // namespace zappy::gui::ui
