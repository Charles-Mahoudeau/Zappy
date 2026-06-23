/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** IEventEmitter
*/

#pragma once

#include "Event.hpp"

namespace zappy::server::game {
class IEventEmitter {
  public:
    IEventEmitter() = default;
    virtual ~IEventEmitter() = default;

    IEventEmitter(const IEventEmitter&) = default;
    IEventEmitter& operator=(const IEventEmitter&) = default;

    IEventEmitter(IEventEmitter&&) = default;
    IEventEmitter& operator=(IEventEmitter&&) = default;

    /// @brief Adds an event to the queue.
    /// @param event The event to add.
    virtual void pushEvent(Event event) = 0;
};
}  // namespace zappy::server::game
