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
#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/render/utils/Rectangle.hpp"

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

float Leaderboard::height(const game::GameState& state) {
    return Widgets::kPanelHeaderHeight + kTextMarginTop + (static_cast<float>(state.teams().size()) * kRowHeight);
}

void Leaderboard::draw(const game::GameState& state, render::Rectangle bounds) {
    Widgets::panel(bounds, "Leaderboard");
    const auto entries = compute(state);
    for (std::size_t i = 0; i < entries.size(); ++i) {
        const auto& entry = entries.at(i);
        const std::string textLeft = std::format("{}. {}", i + 1, entry.team);
        const std::string textRight =
            std::format("{}/{}", entry.playersAtMaxLevel, game::GameState::kPlayersNeededToWin);
        const float progress =
            static_cast<float>(entry.playersAtMaxLevel) / static_cast<float>(game::GameState::kPlayersNeededToWin);
        const render::Rectangle rowBounds{bounds.x() + kBarMarginLeft,
                                          bounds.y() + Widgets::kPanelHeaderHeight + kTextMarginTop +
                                              (static_cast<float>(i) * kRowHeight) + ((kRowHeight - kBarHeight) / 2.0F),
                                          bounds.width() - kBarMarginLeft - kBarMarginRight, kBarHeight};
        Widgets::progressBar(rowBounds, textLeft, textRight, std::min(progress, 1.0F));
    }
}

}  // namespace zappy::gui::ui
