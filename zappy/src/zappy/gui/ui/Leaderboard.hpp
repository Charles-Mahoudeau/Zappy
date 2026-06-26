/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Leaderboard
*/

#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "zappy/gui/render/utils/Rectangle.hpp"

namespace zappy::gui::game {
class GameState;
}

namespace zappy::gui::ui {

struct LeaderboardEntry {
    std::string team;
    std::size_t playersAtMaxLevel;
    std::size_t playersNeeded;
};

class Leaderboard {
  public:
    Leaderboard() = delete;

    [[nodiscard]] static std::vector<LeaderboardEntry> compute(const game::GameState& state);

    [[nodiscard]] static float height(const game::GameState& state);

    static void draw(const game::GameState& state, render::Rectangle bounds);

  private:
    static constexpr float kRowHeight = 24.0F;
    static constexpr float kTextMarginLeft = 8.0F;
    static constexpr float kTextMarginTop = 4.0F;
    static constexpr float kBarMarginLeft = 90.0F;
    static constexpr float kBarMarginRight = 44.0F;
    static constexpr float kBarHeight = 16.0F;
};

}  // namespace zappy::gui::ui
