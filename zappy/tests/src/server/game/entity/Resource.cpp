/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Resource
*/

#include "zappy/server/game/entity/Resource.hpp"

#include <gtest/gtest.h>

#include "zappy/server/game/ResourceType.hpp"

namespace zappy::server::game::entity {
TEST(ResourceTest, ConstructorAndType) {
    const Resource resource{ResourceType::kFood};

    EXPECT_EQ(resource.type(), ResourceType::kFood);
}

TEST(ResourceTest, UpdateDoesNotCrash) {
    Resource resource{ResourceType::kFood};

    EXPECT_NO_THROW(resource.update());
}
}  // namespace zappy::server::game::entity
