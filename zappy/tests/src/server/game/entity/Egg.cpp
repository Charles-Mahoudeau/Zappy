/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** EggTest
*/

#include "zappy/server/game/entity/Egg.hpp"

#include <gtest/gtest.h>

namespace zappy::server::game::entity {
TEST(EggTest, ConstructorAndTeamName) {
    const Egg egg0{0};
    const Egg egg1{1};

    EXPECT_EQ(egg0.teamId(), 0);
    EXPECT_EQ(egg1.teamId(), 1);
}

TEST(EggTest, UpdateDoesNotCrash) {
    Egg egg{0};

    EXPECT_NO_THROW(egg.update());
}
}  // namespace zappy::server::game::entity
