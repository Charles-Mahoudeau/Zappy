/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** WorldMock
*/

#pragma once

#include <gmock/gmock.h>

#include <cstdint>
#include <optional>
#include <string_view>

#include "zappy/server/game/Event.hpp"
#include "zappy/server/game/IEntity.hpp"
#include "zappy/server/game/IEventEmitter.hpp"
#include "zappy/server/game/IGrid.hpp"
#include "zappy/shared/math/Vector2.hpp"

namespace zappy::server::game {
class WorldMock : public IGrid, public IEventEmitter {
  public:
    MOCK_METHOD(std::optional<math::Vector2u>, position, (std::uint64_t id), (const, override));
    MOCK_METHOD(void, setPosition, (std::uint16_t id, math::Vector2u position), (override));
    MOCK_METHOD(void, remove, (std::uint64_t id), (override));

    MOCK_METHOD(void, pushEvent, (Event event), (override));
};
}  // namespace zappy::server::game
