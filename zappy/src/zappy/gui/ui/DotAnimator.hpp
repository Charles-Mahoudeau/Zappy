/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** DotAnimator
*/

#pragma once

#include <string>
#include <string_view>

namespace zappy::gui::ui {

class DotAnimator {
  public:
    explicit DotAnimator(int maxDots = 3);
    ~DotAnimator() = default;

    DotAnimator(const DotAnimator&) = default;
    DotAnimator& operator=(const DotAnimator&) = default;

    DotAnimator(DotAnimator&&) noexcept = default;
    DotAnimator& operator=(DotAnimator&&) noexcept = default;

    void advance();
    void update(float dt);

    [[nodiscard]] std::string text(std::string_view label) const;

  private:
    static constexpr float kIntervalSeconds = 0.4F;

    int _maxDots;
    int _dots{1};
    float _timer{0.0F};
};

}  // namespace zappy::gui::ui
