/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** EggTest
*/

#include "zappy/server/game/entity/Egg.hpp"

#include <gtest/gtest.h>

#include "server/game/WorldMock.hpp"

namespace zappy::server::game::entity {
namespace {
class EggTest : public testing::Test {
  public:
    [[nodiscard]] WorldMock& worldMock() { return _worldMock; }

  private:
    WorldMock _worldMock;
};
}  // namespace

TEST_F(EggTest, ConstructorAndTeamName) {
    const Egg egg0{worldMock(), worldMock(), "team1"};
    const Egg egg1{worldMock(), worldMock(), "team2"};

    EXPECT_EQ(egg0.teamName(), "team1");
    EXPECT_EQ(egg1.teamName(), "team2");
}

TEST_F(EggTest, UpdateDoesNotCrash) {
    Egg egg{worldMock(), worldMock(), "team1"};

    EXPECT_NO_THROW(egg.update());
}
}  // namespace zappy::server::game::entity
