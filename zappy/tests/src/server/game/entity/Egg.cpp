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
    const Egg egg{"Team1"};

    EXPECT_EQ(egg.teamName(), "Team1");
}

TEST(EggTest, UpdateDoesNotCrash) {
    Egg egg{"Team1"};

    EXPECT_NO_THROW(egg.update());
}
}  // namespace zappy::server::game::entity
