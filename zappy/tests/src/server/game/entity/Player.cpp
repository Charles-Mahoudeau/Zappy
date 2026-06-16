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

namespace zappy::server::game::entity {
TEST(PlayerTest, DefaultConstructor) {
    const Player player;

    EXPECT_TRUE(player.alive());
    EXPECT_EQ(player.level(), 1);
    EXPECT_EQ(player.lifetimeLeft(), Player::kDefaultLifetime);
}

TEST(PlayerTest, KillPlayer) {
    Player player;

    player.kill();
    EXPECT_FALSE(player.alive());
}

TEST(PlayerTest, FreezePlayer) {
    Player player;

    EXPECT_FALSE(player.frozen());
    player.freeze(10);
    EXPECT_TRUE(player.frozen());
}

TEST(PlayerTest, LevelUp) {
    Player player;
    const std::expected<std::uint8_t, std::string> result = player.levelUp();

    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 2);
    EXPECT_EQ(player.level(), 2);
}
}  // namespace zappy::server::game::entity
