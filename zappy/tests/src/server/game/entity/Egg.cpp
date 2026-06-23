/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** EggTest
*/

#include "zappy/server/game/entity/Egg.hpp"

#include <gtest/gtest.h>

#include "server/game/WorldMock.hpp"
#include "zappy/server/game/Grid.hpp"

namespace zappy::server::game::entity {
namespace {
class EggTest : public testing::Test {
  public:
    [[nodiscard]] WorldMock& worldMock() { return _worldMock; }
    [[nodiscard]] Grid& grid() { return _grid; }

  private:
    WorldMock _worldMock;
    Grid _grid{{10, 10}};
};
}  // namespace

TEST_F(EggTest, ConstructorAndTeamName) {
    const Egg egg0{grid(), worldMock(), "team1"};
    const Egg egg1{grid(), worldMock(), "team2"};

    EXPECT_EQ(egg0.teamName(), "team1");
    EXPECT_EQ(egg1.teamName(), "team2");
}

TEST_F(EggTest, UpdateDoesNotCrash) {
    Egg egg{grid(), worldMock(), "team1"};

    EXPECT_NO_THROW(egg.update());
}
}  // namespace zappy::server::game::entity
