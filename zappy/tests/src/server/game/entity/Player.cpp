/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Player
*/

#include "zappy/server/game/entity/Player.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <expected>

#include "server/game/WorldMock.hpp"

namespace zappy::server::game::entity {
namespace {
class PlayerTest : public testing::Test {
  public:
    [[nodiscard]] WorldMock& worldMock() { return _worldMock; }

  private:
    WorldMock _worldMock;
};
}  // namespace

TEST_F(PlayerTest, DefaultConstructor) {
    const Player player{worldMock(), worldMock(), "team1"};

    EXPECT_EQ(player.teamName(), "team1");
    EXPECT_TRUE(player.alive());
    EXPECT_EQ(player.level(), 1);
    EXPECT_EQ(player.lifetimeLeft(), Player::kDefaultLifetime);
}

TEST_F(PlayerTest, KillPlayer) {
    Player player{worldMock(), worldMock(), "team1"};

    player.kill();
    EXPECT_FALSE(player.alive());
}

TEST_F(PlayerTest, LevelUp) {
    Player player{worldMock(), worldMock(), "team1"};
    const std::expected<std::uint8_t, std::string> result = player.levelUp();

    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 2);
    EXPECT_EQ(player.level(), 2);
}
}  // namespace zappy::server::game::entity
