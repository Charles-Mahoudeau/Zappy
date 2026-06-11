/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** GameState tests
*/

#include "zappy/gui/game/GameState.hpp"

#include <gtest/gtest.h>

#include <string>

#include "zappy/shared/exception/InvalidArgument.hpp"

namespace {

using zappy::gui::game::GameState;
using zappy::gui::game::Orientation;
using zappy::gui::game::Resources;

TEST(GameStateTest, InitialStateIsNotReady) {
    GameState state;
    EXPECT_FALSE(state.isReady());
    EXPECT_EQ(state.width(), 0U);
    EXPECT_EQ(state.height(), 0U);
}

TEST(GameStateTest, InitialStateHasNoGameOver) {
    GameState state;
    EXPECT_FALSE(state.isGameOver());
    EXPECT_FALSE(state.winner().has_value());
}

TEST(GameStateTest, SetMapSizeUpdatesDimensions) {
    GameState state;
    state.setMapSize(10, 20);
    EXPECT_EQ(state.width(), 10U);
    EXPECT_EQ(state.height(), 20U);
}

TEST(GameStateTest, SetMapSizeResizesTiles) {
    GameState state;
    state.setMapSize(5, 3);
    EXPECT_EQ(state.tiles().size(), 3U);
    EXPECT_EQ(state.tiles().at(0).size(), 5U);
}

TEST(GameStateTest, IsReadyAfterAllTilesReceived) {
    GameState state;
    state.setMapSize(2, 2);
    EXPECT_FALSE(state.isReady());
    state.setTile(0, 0, {});
    state.setTile(1, 0, {});
    state.setTile(0, 1, {});
    EXPECT_FALSE(state.isReady());
    state.setTile(1, 1, {});
    EXPECT_TRUE(state.isReady());
}

TEST(GameStateTest, SetMapSizeResetsReadiness) {
    GameState state;
    state.setMapSize(1, 1);
    state.setTile(0, 0, {});
    EXPECT_TRUE(state.isReady());
    state.setMapSize(2, 2);
    EXPECT_FALSE(state.isReady());
}

TEST(GameStateTest, SetTileStoresResources) {
    GameState state;
    state.setMapSize(3, 3);
    const Resources res{.food = 5, .linemate = 2};
    state.setTile(1, 2, res);
    EXPECT_EQ(state.tiles().at(2).at(1).food, 5U);
    EXPECT_EQ(state.tiles().at(2).at(1).linemate, 2U);
}

TEST(GameStateTest, SetTileOutOfBoundsThrows) {
    GameState state;
    state.setMapSize(3, 3);
    EXPECT_THROW(state.setTile(3, 0, {}), zappy::exception::InvalidArgument);
    EXPECT_THROW(state.setTile(0, 3, {}), zappy::exception::InvalidArgument);
}

TEST(GameStateTest, AddTeamStoresName) {
    GameState state;
    state.addTeam("alpha");
    state.addTeam("beta");
    EXPECT_EQ(state.teams().size(), 2U);
    EXPECT_EQ(state.teams().at(0), "alpha");
    EXPECT_EQ(state.teams().at(1), "beta");
}

TEST(GameStateTest, AddPlayerStoresPlayer) {
    GameState state;
    state.addPlayer(1, 3, 4, Orientation::North, 2, "alpha");
    EXPECT_EQ(state.players().size(), 1U);
    const auto& p = state.players().at(1);
    EXPECT_EQ(p.x, 3U);
    EXPECT_EQ(p.y, 4U);
    EXPECT_EQ(p.orientation, Orientation::North);
    EXPECT_EQ(p.level, 2U);
    EXPECT_EQ(p.team, "alpha");
}

TEST(GameStateTest, SetPlayerPositionUpdatesFields) {
    GameState state;
    state.addPlayer(1, 0, 0, Orientation::North, 1, "alpha");
    state.setPlayerPosition(1, 5, 6, Orientation::East);
    const auto& p = state.players().at(1);
    EXPECT_EQ(p.x, 5U);
    EXPECT_EQ(p.y, 6U);
    EXPECT_EQ(p.orientation, Orientation::East);
}

TEST(GameStateTest, SetPlayerPositionUnknownIdThrows) {
    GameState state;
    EXPECT_THROW(state.setPlayerPosition(99, 0, 0, Orientation::North), zappy::exception::InvalidArgument);
}

TEST(GameStateTest, SetPlayerLevelUpdatesLevel) {
    GameState state;
    state.addPlayer(1, 0, 0, Orientation::North, 1, "alpha");
    state.setPlayerLevel(1, 5);
    EXPECT_EQ(state.players().at(1).level, 5U);
}

TEST(GameStateTest, SetPlayerLevelUnknownIdThrows) {
    GameState state;
    EXPECT_THROW(state.setPlayerLevel(99, 1), zappy::exception::InvalidArgument);
}

TEST(GameStateTest, SetPlayerInventoryUpdatesPositionAndInventory) {
    GameState state;
    state.addPlayer(1, 0, 0, Orientation::North, 1, "alpha");
    const Resources inv{.food = 3, .thystame = 1};
    state.setPlayerInventory(1, 7, 8, inv);
    const auto& p = state.players().at(1);
    EXPECT_EQ(p.x, 7U);
    EXPECT_EQ(p.y, 8U);
    EXPECT_EQ(p.inventory.food, 3U);
    EXPECT_EQ(p.inventory.thystame, 1U);
}

TEST(GameStateTest, SetPlayerInventoryUnknownIdThrows) {
    GameState state;
    EXPECT_THROW(state.setPlayerInventory(99, 0, 0, {}), zappy::exception::InvalidArgument);
}

TEST(GameStateTest, RemovePlayerErasesPlayer) {
    GameState state;
    state.addPlayer(1, 0, 0, Orientation::North, 1, "alpha");
    state.removePlayer(1);
    EXPECT_EQ(state.players().size(), 0U);
}

TEST(GameStateTest, AddEggStoresEgg) {
    GameState state;
    state.addEgg(10, 1, 4, 5);
    EXPECT_EQ(state.eggs().size(), 1U);
    const auto& e = state.eggs().at(10);
    EXPECT_EQ(e.playerId, 1U);
    EXPECT_EQ(e.x, 4U);
    EXPECT_EQ(e.y, 5U);
}

TEST(GameStateTest, RemoveEggErasesEgg) {
    GameState state;
    state.addEgg(10, 1, 0, 0);
    state.removeEgg(10);
    EXPECT_EQ(state.eggs().size(), 0U);
}

TEST(GameStateTest, SetTimeUnitStoresValue) {
    GameState state;
    state.setTimeUnit(100);
    EXPECT_EQ(state.timeUnit(), 100U);
}

TEST(GameStateTest, SetWinnerSetsGameOver) {
    GameState state;
    state.setWinner("alpha");
    EXPECT_TRUE(state.isGameOver());
    EXPECT_EQ(state.winner().value_or(""), "alpha");
}

}  // namespace
