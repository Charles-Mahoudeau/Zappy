/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** IWorldEventEmitter
*/

#pragma once

#include "WorldEvent.hpp"

namespace zappy::server::game {
class IWorldEventEmitter {
  public:
    IWorldEventEmitter() = default;
    virtual ~IWorldEventEmitter() = default;

    IWorldEventEmitter(const IWorldEventEmitter&) = default;
    IWorldEventEmitter& operator=(const IWorldEventEmitter&) = default;

    IWorldEventEmitter(IWorldEventEmitter&&) = default;
    IWorldEventEmitter& operator=(IWorldEventEmitter&&) = default;

    /// @brief Adds an event to the queue.
    /// @param event The event to add.
    virtual void pushEvent(WorldEvent event) = 0;
};
}  // namespace zappy::server::game
