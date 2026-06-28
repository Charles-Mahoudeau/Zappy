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
std::string WireHelper::tileToCommands(const Tile& tile) {
    return std::format("bct {} {} {}\n", tile.position().x, tile.position().y, tile.inventory().string());
}

std::string WireHelper::worldToTileCommands(const World& world) {
    std::stringstream result;
    const math::Vector2u worldSize = world.size();

    for (std::uint32_t x = 0; x < worldSize.x; ++x) {
        for (std::uint32_t y = 0; y < worldSize.y; ++y) {
            result << tileToCommands(world.grid().tile({x, y}));
        }
    }
    return result.str();
}

std::string WireHelper::worldToEggCommands(const World& world) {
    std::stringstream result;

    for (const entity::Egg* egg : world.entityDatabase().viewAll<entity::Egg>()) {
        if (egg == nullptr) {
            continue;
        }
        if (std::optional<std::uint64_t> parentPlayerId = egg->parentPlayerId()) {
            result << std::format("enw #{} #{} {} {}\n", egg->id(), *parentPlayerId, egg->position().x,
                                  egg->position().y);
        } else {
            result << std::format("enw #{} #-1 {} {}\n", egg->id(), egg->position().x, egg->position().y);
        }
    }
    return result.str();
}

std::string WireHelper::worldToPlayerCommands(const World& world) {
    std::stringstream result;

    for (const entity::Player* player : world.entityDatabase().viewAll<entity::Player>()) {
        if (player == nullptr) {
            continue;
        }
        result << std::format("pnw #{} {} {} {} {} {}\n", player->id(), player->position().x, player->position().y,
                              std::to_underlying(player->orientation()) + 1, player->level(), player->teamName());
        result << std::format("pin #{} {} {} {}\n", player->id(), player->position().x, player->position().y,
                              player->inventory().string());
        result << std::format("plv #{} {}\n", player->id(), player->level());
    }
    return result.str();
}
}  // namespace zappy::server::game
