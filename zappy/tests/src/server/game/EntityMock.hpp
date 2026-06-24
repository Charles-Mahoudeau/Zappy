/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** EntityMock
*/

#pragma once

#include <gmock/gmock.h>

#include <cstdint>
#include <string_view>

#include "zappy/server/game/IEntity.hpp"
#include "zappy/shared/math/Vector2.hpp"

namespace zappy::server::game {
class EntityMock : public IEntity {
  public:
    MOCK_METHOD(std::uint64_t, id, (), (const, override));
    MOCK_METHOD(void, setId, (std::uint64_t id), (override));

    MOCK_METHOD(math::Vector2u, position, (), (const, override));
    MOCK_METHOD(void, setPosition, (math::Vector2u position), (override));

    MOCK_METHOD(std::string_view, teamName, (), (const, override));

    MOCK_METHOD(void, update, (), (override));
};
}  // namespace zappy::server::game
