/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Event
*/

#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "Inventory.hpp"
#include "zappy/server/game/ResourceType.hpp"
#include "zappy/shared/math/Direction.hpp"
#include "zappy/shared/math/Vector2.hpp"

namespace zappy::server::game {
struct TileInventoryEvent {
    math::Vector2u position;
    std::reference_wrapper<const Inventory> inventory;
};

struct PlayerConnectionEvent {
    std::uint64_t playerId{};
    math::Vector2u position;
    math::Direction orientation{};
    std::uint8_t level{};
    std::string teamName;
};

struct PlayerPositionEvent {
    std::uint64_t playerId{};
    math::Vector2u position;
    math::Direction orientation{};
};

struct PlayerLevelEvent {
    std::uint64_t playerId{};
    std::uint8_t level{};
};

struct PlayerInventoryEvent {
    std::uint64_t playerId{};
    std::reference_wrapper<const Inventory> inventory;
};

struct PlayerExpulsionEvent {
    std::uint64_t playerId{};
};

struct PlayerBroadcastEvent {
    std::uint64_t playerId{};
    std::string message;
};

struct IncantationBeginEvent {
    std::vector<std::uint64_t> playerIds;
    math::Vector2u position;
};

struct IncantationEndEvent {
    math::Vector2u position;
    bool success{};
};

struct PlayerEggLayingEvent {
    std::uint64_t playerId{};
};

struct PlayerResourceDropEvent {
    std::uint64_t playerId{};
    ResourceType resourceType{};
};

struct PlayerResourceCollectEvent {
    std::uint64_t playerId{};
    ResourceType resourceType{};
};

struct PlayerDeathEvent {
    std::uint64_t playerId{};
};

struct EggLaidEvent {
    std::optional<std::uint64_t> playerId;
    std::uint64_t eggId{};
    math::Vector2u position;
};

struct EggConnectionEvent {
    std::uint64_t eggId{};
};

struct EggDeathEvent {
    std::uint64_t eggId{};
};

struct GameEndEvent {
    std::string teamName;
};

using Event =
    std::variant<TileInventoryEvent, PlayerConnectionEvent, PlayerPositionEvent, PlayerLevelEvent, PlayerInventoryEvent,
                 PlayerExpulsionEvent, PlayerBroadcastEvent, IncantationBeginEvent, IncantationEndEvent,
                 PlayerEggLayingEvent, PlayerResourceDropEvent, PlayerResourceCollectEvent, PlayerDeathEvent,
                 EggLaidEvent, EggConnectionEvent, EggDeathEvent, GameEndEvent>;
}  // namespace zappy::server::game
