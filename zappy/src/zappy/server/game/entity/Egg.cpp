/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Egg
*/

#include "Egg.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <utility>

#include "zappy/server/game/AEntity.hpp"
#include "zappy/server/game/IEventEmitter.hpp"
#include "zappy/server/game/IGrid.hpp"

namespace zappy::server::game::entity {
Egg::Egg(IGrid& grid, IEventEmitter& eventEmitter, std::string teamName,
         const std::optional<std::uint64_t> parentPlayerId)
    : AEntity{grid, eventEmitter, std::move(teamName)}, _parentPlayerId{parentPlayerId} {}

void Egg::update() {
    // Empty
}

std::optional<std::uint64_t> Egg::parentPlayerId() const { return _parentPlayerId; }
}  // namespace zappy::server::game::entity
