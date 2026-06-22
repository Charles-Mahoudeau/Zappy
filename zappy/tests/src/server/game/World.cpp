/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** World
*/

#include "zappy/server/game/World.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <iterator>
#include <string_view>
#include <tuple>

#include "zappy/server/game/ResourceType.hpp"
#include "zappy/server/game/Tile.hpp"
#include "zappy/server/game/entity/Egg.hpp"
#include "zappy/shared/exception/InvalidArgument.hpp"
#include "zappy/shared/exception/InvalidState.hpp"
#include "zappy/shared/exception/OutOfRange.hpp"
#include "zappy/shared/io/Logger.hpp"

namespace {
class WorldTest : public testing::Test {
  public:
    [[nodiscard]] zappy::io::Logger logger(const std::string_view testName) const { return _logger.derive(testName); }

  private:
    zappy::io::Logger _logger{"World"};
};
}  // namespace

TEST_F(WorldTest, Size) {
    const zappy::server::game::World world1{{
        .size = {69, 42},
        .teamCount = 1,
        .playersPerTeam = 1,
        .logger = logger("Size"),
    }};

    EXPECT_EQ(world1.size(), (zappy::math::Vector2u{69, 42}));

    const zappy::server::game::World world2{{
        .size = {3, 10},
        .teamCount = 1,
        .playersPerTeam = 1,
    }};

    EXPECT_EQ(world2.size(), (zappy::math::Vector2u{3, 10}));

    EXPECT_THROW((zappy::server::game::World{{
                     .size = {0, 0},
                     .teamCount = 1,
                     .playersPerTeam = 1,
                 }}),
                 zappy::exception::OutOfRange);
    EXPECT_THROW((zappy::server::game::World{{
                     .size = {1, 0},
                     .teamCount = 1,
                     .playersPerTeam = 1,
                 }}),
                 zappy::exception::OutOfRange);
    EXPECT_THROW((zappy::server::game::World{{
                     .size = {0, 1},
                     .teamCount = 1,
                     .playersPerTeam = 1,
                 }}),
                 zappy::exception::OutOfRange);
}

TEST_F(WorldTest, TileAccess) {
    zappy::server::game::World world{{
        .size = {69, 42},
        .teamCount = 1,
        .playersPerTeam = 1,
        .logger = logger("TileAccess"),
    }};

    EXPECT_NO_THROW(std::ignore = world.tile({0, 0}));
    EXPECT_NO_THROW(std::ignore = world.tile({68, 41}));
    EXPECT_THROW(std::ignore = world.tile({69, 41}), zappy::exception::OutOfRange);
    EXPECT_THROW(std::ignore = world.tile({68, 42}), zappy::exception::OutOfRange);
}

TEST_F(WorldTest, ResourceCounting) {
    zappy::server::game::World world{{
        .size = {10, 10},
        .teamCount = 1,
        .playersPerTeam = 1,
        .logger = logger("ResourceCounting"),
    }};

    EXPECT_EQ(world.countResources(zappy::server::game::ResourceType::kFood), 0);
    std::ignore = world.spawnResource(zappy::server::game::ResourceType::kFood);
    EXPECT_EQ(world.countResources(zappy::server::game::ResourceType::kFood), 1);
}

TEST_F(WorldTest, EggSpawning) {
    zappy::server::game::World world{{
        .size = {10, 10},
        .teamCount = 1,
        .playersPerTeam = 1,
        .logger = logger("EggSpawning"),
    }};

    EXPECT_EQ(world.count<zappy::server::game::entity::Egg>(), 1);
    std::ignore = world.spawnEgg(1);
    EXPECT_EQ(world.count<zappy::server::game::entity::Egg>(), 2);
}

TEST_F(WorldTest, PlayerAccess) {
    zappy::server::game::World world{{
        .size = {10, 10},
        .teamCount = 1,
        .playersPerTeam = 1,
        .logger = logger("PlayerAccess"),
    }};

    // Check that there are no players initially
    EXPECT_EQ(std::ranges::distance(world.players(0)), 0);

    // Hatch an egg to create a player
    const auto playerResult = world.hatchRandomEgg(0);
    ASSERT_TRUE(playerResult.has_value());

    // Check that there is now a player
    auto players = world.players(0);
    EXPECT_EQ(std::ranges::distance(players), 1);
    EXPECT_EQ(world.entityDatabase().id(**players.begin()), playerResult.value());

    // Check const version
    const auto& constWorld = world;
    auto constPlayers = constWorld.players(0);
    EXPECT_EQ(std::ranges::distance(constPlayers), 1);
    EXPECT_EQ(constWorld.entityDatabase().id(**constPlayers.begin()), playerResult.value());
}

TEST_F(WorldTest, MoveBy) {
    zappy::server::game::World world{{
        .size = {10, 10},
        .teamCount = 1,
        .playersPerTeam = 1,
        .logger = logger("MoveBy"),
    }};

    // Hatch an egg to create a player
    const auto playerResult = world.hatchRandomEgg(0);
    ASSERT_TRUE(playerResult.has_value());
    const std::uint64_t playerId = playerResult.value();

    // Get initial position
    const zappy::server::game::Tile* tile = world.tile(playerId);
    ASSERT_NE(tile, nullptr);
    const zappy::math::Vector2u initialPos = tile->position();

    // Move by {1, 1}
    const zappy::math::Vector2u newPos = world.moveBy(playerId, {1, 1});

    // Check that the player is actually on the new tile
    const zappy::server::game::Tile* newTile = world.tile(playerId);
    ASSERT_NE(newTile, nullptr);
    EXPECT_EQ(newTile->position(), newPos);
    EXPECT_EQ(newPos - initialPos, (zappy::math::Vector2u{1, 1}));

    // Test wrapping
    // Move to (0, 0)
    const zappy::math::Vector2u posAtOrigin =
        world.moveBy(playerId, {0 - static_cast<int>(newPos.x), 0 - static_cast<int>(newPos.y)});
    EXPECT_EQ(posAtOrigin, (zappy::math::Vector2u{0, 0}));

    // Move by {-1, -1}, should wrap to (9, 9)
    const zappy::math::Vector2u wrappedPos = world.moveBy(playerId, {-1, -1});
    EXPECT_EQ(wrappedPos, (zappy::math::Vector2u{9, 9}));

    const zappy::server::game::Tile* wrappedTile = world.tile(playerId);
    ASSERT_NE(wrappedTile, nullptr);
    EXPECT_EQ(wrappedTile->position(), (zappy::math::Vector2u{9, 9}));
}

TEST_F(WorldTest, Position) {
    zappy::server::game::World world{{
        .size = {10, 10},
        .teamCount = 1,
        .playersPerTeam = 1,
        .logger = logger("Position"),
    }};

    const auto playerResult = world.hatchRandomEgg(0);
    ASSERT_TRUE(playerResult.has_value());
    const std::uint64_t playerId = playerResult.value();

    const zappy::server::game::Tile* tile = world.tile(playerId);
    ASSERT_NE(tile, nullptr);

    EXPECT_EQ(world.position(playerId), tile->position());
    EXPECT_THROW(std::ignore = world.position(9999), zappy::exception::InvalidState);
}

TEST_F(WorldTest, MoveTo) {
    zappy::server::game::World world{{
        .size = {10, 10},
        .teamCount = 1,
        .playersPerTeam = 1,
        .logger = logger("MoveTo"),
    }};

    const auto playerResult = world.hatchRandomEgg(0);
    ASSERT_TRUE(playerResult.has_value());
    const std::uint64_t playerId = playerResult.value();

    const zappy::math::Vector2u targetPos = {5, 5};
    world.moveTo(playerId, targetPos);
    EXPECT_EQ(world.position(playerId), targetPos);

    EXPECT_THROW(world.moveTo(playerId, {10, 10}), zappy::exception::InvalidArgument);
}
