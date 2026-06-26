/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** EventHelper
*/

#include "EventHelper.hpp"

#include <format>
#include <sstream>
#include <string>
#include <utility>
#include <variant>

#include "Event.hpp"

namespace {
template <class... Ts>
struct overloads : Ts... {  // NOLINT(*-multiple-inheritance)
    using Ts::operator()...;
};
}  // namespace

namespace zappy::server::game {
std::string EventHelper::toWire(const Event& event) {
    return std::visit(
        overloads{
            [](const TileInventoryEvent& e) {
                return std::format("bct {} {} {}\n", e.position.x, e.position.y, e.inventory.string());
            },
            [](const PlayerConnectionEvent& e) {
                return std::format("pnw #{} {} {} {} {} {}\n", e.playerId, e.position.x, e.position.y,
                                   std::to_underlying(e.orientation) + 1, e.level, e.teamName);
            },
            [](const PlayerPositionEvent& e) {
                return std::format("ppo #{} {} {} {}\n", e.playerId, e.position.x, e.position.y,
                                   std::to_underlying(e.orientation) + 1);
            },
            [](const PlayerLevelEvent& e) { return std::format("plv #{} {}\n", e.playerId, e.level); },
            [](const PlayerInventoryEvent& e) {
                return std::format("pin #{} {} {} {}\n", e.playerId, e.position.x, e.position.y, e.inventory.string());
            },
            [](const PlayerExpulsionEvent& e) { return std::format("pex #{}\n", e.playerId); },
            [](const PlayerBroadcastEvent& e) { return std::format("pbc #{} {}\n", e.playerId, e.message); },
            [](const IncantationBeginEvent& e) {
                std::stringstream playersStringStream;

                for (const auto& playerId : e.playerIds) {
                    playersStringStream << " #" << playerId;
                }
                return std::format("pic {} {} {}{}\n", e.position.x, e.position.y, e.level, playersStringStream.str());
            },
            [](const IncantationEndEvent& e) {
                return std::format("pie {} {} {}\n", e.position.x, e.position.y, e.success ? 1 : 0);
            },
            [](const PlayerEggLayingEvent& e) { return std::format("pfk #{}\n", e.playerId); },
            [](const PlayerResourceDropEvent& e) {
                return std::format("pdr #{} {}\n", e.playerId, std::to_underlying(e.resourceType));
            },
            [](const PlayerResourceCollectEvent& e) {
                return std::format("pgt #{} {}\n", e.playerId, std::to_underlying(e.resourceType));
            },
            [](const PlayerDeathEvent& e) { return std::format("pdi #{}\n", e.playerId); },
            [](const EggLaidEvent& e) {
                std::string playerIdStr = "-1";

                if (e.playerId.has_value()) {
                    playerIdStr = std::format("{}", *e.playerId);
                }
                return std::format("enw #{} #{} {} {}\n", e.eggId, playerIdStr, e.position.x, e.position.y);
            },
            [](const EggConnectionEvent& e) { return std::format("ebo #{}\n", e.eggId); },
            [](const EggDeathEvent& e) { return std::format("edi #{}\n", e.eggId); },
            [](const GameEndEvent& e) { return std::format("seg {}\n", e.teamName); },
        },
        event);
}
}  // namespace zappy::server::game
