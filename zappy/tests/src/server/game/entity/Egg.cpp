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
    [[nodiscard]] Timer& timer() { return _timer; }
    [[nodiscard]] WorldMock& worldMock() { return _worldMock; }
    [[nodiscard]] Grid& grid() { return _grid; }

  private:
    Timer _timer;
    WorldMock _worldMock;
    Grid _grid{{10, 10}};
};
}  // namespace

TEST_F(EggTest, ConstructorAndTeamName) {
    const Egg egg0{timer(), grid(), worldMock(), "team1"};
    const Egg egg1{timer(), grid(), worldMock(), "team2"};

    EXPECT_EQ(egg0.teamName(), "team1");
    EXPECT_EQ(egg1.teamName(), "team2");
}
}  // namespace zappy::server::game::entity
