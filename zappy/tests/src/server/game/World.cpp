/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** World
*/

#include "zappy/server/game/World.hpp"

#include <gtest/gtest.h>

#include <iterator>
#include <string_view>
#include <tuple>

#include "zappy/server/game/ResourceType.hpp"
#include "zappy/server/game/Tile.hpp"
#include "zappy/server/game/entity/Egg.hpp"
#include "zappy/shared/exception/OutOfRange.hpp"
#include "zappy/shared/io/Logger.hpp"
#include "zappy/shared/math/Vector2.hpp"

namespace {
class WorldTest : public testing::Test {
  public:
    [[nodiscard]] zappy::server::Timer& timer() { return _timer; }
    [[nodiscard]] zappy::io::Logger logger(const std::string_view testName) const { return _logger.derive(testName); }

  private:
    zappy::server::Timer _timer;
    zappy::io::Logger _logger{"World"};
};
}  // namespace

TEST_F(WorldTest, Size) {
    const zappy::server::game::World world1{
        {69, 42},
        timer(),
        logger("Size"),
    };

    EXPECT_EQ(world1.size(), (zappy::math::Vector2u{69, 42}));

    const zappy::server::game::World world2{
        {3, 10},
        timer(),
        logger("Size"),
    };

    EXPECT_EQ(world2.size(), (zappy::math::Vector2u{3, 10}));

    EXPECT_THROW((zappy::server::game::World{{0, 0}, timer()}), zappy::exception::OutOfRange);
    EXPECT_THROW((zappy::server::game::World{{1, 0}, timer()}), zappy::exception::OutOfRange);
    EXPECT_THROW((zappy::server::game::World{{0, 1}, timer()}), zappy::exception::OutOfRange);
}

TEST_F(WorldTest, ResourceCounting) {
    zappy::server::game::World world{
        {10, 10},
        timer(),
        logger("ResourceCounting"),
    };

    EXPECT_EQ(world.countResources(zappy::server::game::ResourceType::kFood), 0);
    world.spawnResource(zappy::server::game::ResourceType::kFood);
    EXPECT_EQ(world.countResources(zappy::server::game::ResourceType::kFood), 1);
}

TEST_F(WorldTest, EggSpawning) {
    zappy::server::game::World world{
        {10, 10},
        timer(),
        logger("EggSpawning"),
    };

    EXPECT_EQ(world.count<zappy::server::game::entity::Egg>(), 0);
    std::ignore = world.spawnEgg("team1");
    EXPECT_EQ(world.count<zappy::server::game::entity::Egg>(), 1);
}

TEST_F(WorldTest, TeamCount) {
    zappy::server::game::World world{
        {10, 10},
        timer(),
        logger("EggSpawning"),
    };

    std::ignore = world.spawnEgg("team1");
    std::ignore = world.spawnEgg("team1");
    std::ignore = world.spawnEgg("team1");
    std::ignore = world.spawnEgg("team2");

    EXPECT_EQ(world.eggCount("team1"), 3);
    EXPECT_EQ(world.eggCount("team2"), 1);
    EXPECT_EQ(world.eggCount("team3"), 0);
}

TEST_F(WorldTest, PlayerAccess) {
    zappy::server::game::World world{
        {10, 10},
        timer(),
        logger("PlayerAccess"),
    };

    // Check that there are no players initially
    EXPECT_EQ(std::ranges::distance(world.players("team1")), 0);

    // Hatch an egg to create a player
    std::ignore = world.spawnEgg("team1");
    const auto playerResult = world.hatchRandomEgg("team1");
    ASSERT_TRUE(playerResult.has_value());

    // Check that there is now a player
    auto players = world.players("team1");
    EXPECT_EQ(std::ranges::distance(players), 1);
    EXPECT_EQ(world.entityDatabase().id(**players.begin()), playerResult.value());

    // Check const version
    const auto& constWorld = world;
    auto constPlayers = constWorld.players("team1");
    EXPECT_EQ(std::ranges::distance(constPlayers), 1);
    EXPECT_EQ(constWorld.entityDatabase().id(**constPlayers.begin()), playerResult.value());
}
