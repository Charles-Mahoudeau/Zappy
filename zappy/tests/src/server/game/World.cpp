/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** World
*/

#include "zappy/server/game/World.hpp"

#include <gtest/gtest.h>

#include <string_view>
#include <tuple>

#include "zappy/server/game/ResourceType.hpp"
#include "zappy/server/game/entity/Egg.hpp"
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
        .width = 69,
        .height = 42,
        .teamCount = 1,
        .playersPerTeam = 1,
        .logger = logger("Size"),
    }};

    EXPECT_EQ(world1.width(), 69);
    EXPECT_EQ(world1.height(), 42);

    const zappy::server::game::World world2{{
        .width = 3,
        .height = 10,
        .teamCount = 1,
        .playersPerTeam = 1,
    }};

    EXPECT_EQ(world2.width(), 3);
    EXPECT_EQ(world2.height(), 10);

    EXPECT_THROW((zappy::server::game::World{{
                     .width = 0,
                     .height = 0,
                     .teamCount = 1,
                     .playersPerTeam = 1,
                 }}),
                 zappy::exception::OutOfRange);
    EXPECT_THROW((zappy::server::game::World{{
                     .width = 1,
                     .height = 0,
                     .teamCount = 1,
                     .playersPerTeam = 1,
                 }}),
                 zappy::exception::OutOfRange);
    EXPECT_THROW((zappy::server::game::World{{
                     .width = 0,
                     .height = 1,
                     .teamCount = 1,
                     .playersPerTeam = 1,
                 }}),
                 zappy::exception::OutOfRange);
}

TEST_F(WorldTest, TileAccess) {
    zappy::server::game::World world{{
        .width = 69,
        .height = 42,
        .teamCount = 1,
        .playersPerTeam = 1,
        .logger = logger("TileAccess"),
    }};

    EXPECT_NO_THROW(std::ignore = world.tile(0, 0));
    EXPECT_NO_THROW(std::ignore = world.tile(68, 41));
    EXPECT_THROW(std::ignore = world.tile(69, 41), zappy::exception::OutOfRange);
    EXPECT_THROW(std::ignore = world.tile(68, 42), zappy::exception::OutOfRange);
}

TEST_F(WorldTest, ResourceCounting) {
    zappy::server::game::World world{{
        .width = 10,
        .height = 10,
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
        .width = 10,
        .height = 10,
        .teamCount = 1,
        .playersPerTeam = 1,
        .logger = logger("EggSpawning"),
    }};

    EXPECT_EQ(world.count<zappy::server::game::entity::Egg>(), 1);
    std::ignore = world.spawnEgg(1);
    EXPECT_EQ(world.count<zappy::server::game::entity::Egg>(), 2);
}
