/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Leaderboard tests
*/

#include "zappy/gui/ui/Leaderboard.hpp"

#include <gtest/gtest.h>

#include <cstdint>

#include "zappy/gui/game/GameState.hpp"

namespace ui = zappy::gui::ui;

namespace {

class LeaderboardTest : public ::testing::Test {
  public:
    zappy::gui::game::GameState _state;
};

TEST_F(LeaderboardTest, ComputeIsEmptyWithNoTeams) { EXPECT_TRUE(ui::Leaderboard::compute(_state).empty()); }

TEST_F(LeaderboardTest, ComputeCountsZeroForTeamWithNoQualifyingPlayers) {
    _state.addTeam("alpha");
    const auto entries = ui::Leaderboard::compute(_state);
    ASSERT_EQ(entries.size(), 1U);
    EXPECT_EQ(entries.at(0).team, "alpha");
    EXPECT_EQ(entries.at(0).playersAtMaxLevel, 0U);
    EXPECT_EQ(entries.at(0).playersNeeded, zappy::gui::game::GameState::kPlayersNeededToWin);
}

TEST_F(LeaderboardTest, ComputeSortsByPlayersAtMaxLevelDescending) {
    using zappy::gui::game::Orientation;
    constexpr auto kMaxLevel = zappy::gui::game::GameState::kMaxLevel;

    _state.addTeam("alpha");
    _state.addTeam("beta");
    _state.addPlayer(1, 0, 0, Orientation::North, kMaxLevel, "beta");
    _state.addPlayer(2, 0, 0, Orientation::North, kMaxLevel, "alpha");
    _state.addPlayer(3, 0, 0, Orientation::North, kMaxLevel, "alpha");

    const auto entries = ui::Leaderboard::compute(_state);
    ASSERT_EQ(entries.size(), 2U);
    EXPECT_EQ(entries.at(0).team, "alpha");
    EXPECT_EQ(entries.at(0).playersAtMaxLevel, 2U);
    EXPECT_EQ(entries.at(1).team, "beta");
    EXPECT_EQ(entries.at(1).playersAtMaxLevel, 1U);
}

TEST_F(LeaderboardTest, ComputeBreaksTiesByTeamName) {
    _state.addTeam("zeta");
    _state.addTeam("alpha");

    const auto entries = ui::Leaderboard::compute(_state);
    ASSERT_EQ(entries.size(), 2U);
    EXPECT_EQ(entries.at(0).team, "alpha");
    EXPECT_EQ(entries.at(1).team, "zeta");
}

TEST_F(LeaderboardTest, ComputeNeededCountReachesZeroOnceMet) {
    using zappy::gui::game::Orientation;
    constexpr auto kMaxLevel = zappy::gui::game::GameState::kMaxLevel;
    constexpr auto kNeeded = zappy::gui::game::GameState::kPlayersNeededToWin;

    _state.addTeam("alpha");
    for (std::uint32_t i = 0; i < kNeeded; ++i) {
        _state.addPlayer(i, 0, 0, Orientation::North, kMaxLevel, "alpha");
    }

    const auto entries = ui::Leaderboard::compute(_state);
    ASSERT_EQ(entries.size(), 1U);
    EXPECT_EQ(entries.at(0).playersAtMaxLevel, kNeeded);
    EXPECT_EQ(entries.at(0).playersNeeded, 0U);
}

}  // namespace
