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

#include "utils/Rectangle.hpp"

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

    static void draw(const game::GameState& state, Rectangle bounds);
};

}  // namespace zappy::gui::ui
