/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** WorldMock
*/

#pragma once

#include <gmock/gmock.h>

#include "zappy/server/game/Event.hpp"
#include "zappy/server/game/IEventEmitter.hpp"

namespace zappy::server::game {
class WorldMock : public IEventEmitter {
  public:
    MOCK_METHOD(void, pushEvent, (Event event), (override));
};
}  // namespace zappy::server::game
