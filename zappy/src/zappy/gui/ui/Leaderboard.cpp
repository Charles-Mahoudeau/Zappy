/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Leaderboard
*/

#include "Leaderboard.hpp"

#include <algorithm>
#include <cstddef>
#include <format>
#include <string>
#include <vector>

#include "Widgets.hpp"
#include "utils/Rectangle.hpp"
#include "zappy/gui/game/GameState.hpp"

namespace zappy::gui::ui {

std::vector<LeaderboardEntry> Leaderboard::compute(const game::GameState& state) {
    std::vector<LeaderboardEntry> entries;
    entries.reserve(state.teams().size());

    for (const auto& team : state.teams()) {
        const std::size_t count = state.playersAtMaxLevel(team);
        const std::size_t needed =
            count >= game::GameState::kPlayersNeededToWin ? 0 : game::GameState::kPlayersNeededToWin - count;
        entries.push_back(LeaderboardEntry{.team = team, .playersAtMaxLevel = count, .playersNeeded = needed});
    }

    std::ranges::sort(entries, [](const LeaderboardEntry& lhs, const LeaderboardEntry& rhs) {
        if (lhs.playersAtMaxLevel != rhs.playersAtMaxLevel) {
            return lhs.playersAtMaxLevel > rhs.playersAtMaxLevel;
        }
        return lhs.team < rhs.team;
    });

    return entries;
}

void Leaderboard::draw(const game::GameState& state, Rectangle bounds) {
    static constexpr float kRowHeight = 24.0F;
    static constexpr float kTextMarginLeft = 8.0F;
    static constexpr float kTextMarginTop = 4.0F;

    Widgets::panel(bounds, "Leaderboard");
    const auto entries = compute(state);
    for (std::size_t i = 0; i < entries.size(); ++i) {
        const auto& entry = entries.at(i);
        const std::string text =
            std::format("{}. {} - {}/{}", i + 1, entry.team, entry.playersAtMaxLevel, entry.playersNeeded);
        const Rectangle rowBounds{bounds.x() + kTextMarginLeft, bounds.y() + (static_cast<float>(i + 1) * kRowHeight),
                                  bounds.width(), kRowHeight};
        Widgets::label(rowBounds, text);
    }
}

}  // namespace zappy::gui::ui
