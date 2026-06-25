/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** WireHelper
*/

#include "WireHelper.hpp"

#include <cstdint>
#include <format>
#include <optional>
#include <sstream>
#include <string>

#include "Tile.hpp"
#include "entity/Egg.hpp"

namespace zappy::server::game {
std::string WireHelper::tileToBctCommand(const Tile& tile) {
    return std::format("bct {} {} {}\n", tile.position().x, tile.position().y, tile.inventory().string());
}

std::string WireHelper::worldToBctCommands(const World& world) {
    std::stringstream result;
    const math::Vector2u worldSize = world.size();

    for (std::uint32_t x = 0; x < worldSize.x; ++x) {
        for (std::uint32_t y = 0; y < worldSize.y; ++y) {
            result << tileToBctCommand(world.grid().tile({x, y}));
        }
    }
    return result.str();
}

std::string WireHelper::worldToEnwCommands(const World& world) {
    std::stringstream result;

    for (const entity::Egg* egg : world.entityDatabase().viewAll<entity::Egg>()) {
        if (egg == nullptr) {
            continue;
        }
        if (std::optional<std::uint32_t> parentPlayerId = egg->parentPlayerId()) {
            result << std::format("enw #{} #{} {} {}\n", egg->id(), *parentPlayerId, egg->position().x,
                                  egg->position().y);
        } else {
            result << std::format("enw #{} #-1 {} {}\n", egg->id(), egg->position().x, egg->position().y);
        }
    }
    return result.str();
}
}  // namespace zappy::server::game
