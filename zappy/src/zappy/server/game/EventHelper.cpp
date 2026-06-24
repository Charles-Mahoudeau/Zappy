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
            [](const TileInventoryEvent& e) -> std::string {
                return std::format("bct {} {} {}", e.position.x, e.position.y, e.inventory.get().string());
            },
            [](const PlayerConnectionEvent& e) -> std::string {
                return std::format("pnw #{} {} {} {} {} {}", e.playerId, e.position.x, e.position.y,
                                   std::to_underlying(e.orientation) + 1, e.level, e.teamName);
            },
            [](const PlayerPositionEvent& e) -> std::string {
                return std::format("ppo #{} {} {} {}", e.playerId, e.position.x, e.position.y,
                                   std::to_underlying(e.orientation) + 1);
            },
            [](const PlayerLevelEvent& e) -> std::string { return std::format("plv #{} {}", e.playerId, e.level); },
            [](const PlayerInventoryEvent& e) -> std::string {
                return std::format("pin #{} {} {} {}", e.playerId, e.position.x, e.position.y,
                                   e.inventory.get().string());
            },
            [](const PlayerExpulsionEvent& e) -> std::string { return std::format("pex #{}", e.playerId); },
            [](const PlayerBroadcastEvent& e) -> std::string {
                return std::format("pex #{} {}", e.playerId, e.message);
            },
            [](const IncantationBeginEvent& e) -> std::string {
                std::stringstream playersStringStream;

                for (const auto& playerId : e.playerIds) {
                    playersStringStream << " #" << playerId;
                }
                return std::format("pic {} {} {}{}", e.position.x, e.position.y, e.level, playersStringStream.str());
            },
            [](const IncantationEndEvent& e) -> std::string {
                return std::format("pie {} {} {}", e.position.x, e.position.y, e.success ? 1 : 0);
            },
            [](const PlayerEggLayingEvent& e) -> std::string { return std::format("pfk #{}", e.playerId); },
            [](const PlayerResourceDropEvent& e) -> std::string {
                return std::format("pdr #{} {}", e.playerId, std::to_underlying(e.resourceType));
            },
            [](const PlayerResourceCollectEvent& e) -> std::string {
                return std::format("pgt #{} {}", e.playerId, std::to_underlying(e.resourceType));
            },
            [](const PlayerDeathEvent& e) -> std::string { return std::format("pdi #{}", e.playerId); },
            [](const EggLaidEvent& e) -> std::string {
                return std::format("enw #{} #{} {} {}", e.eggId, e.playerId.value_or(-1), e.position.x, e.position.y);
            },
            [](const EggConnectionEvent& e) -> std::string { return std::format("ebo #{}", e.eggId); },
            [](const EggDeathEvent& e) -> std::string { return std::format("edi #{}", e.eggId); },
            [](const GameEndEvent& e) -> std::string { return std::format("seg {}", e.teamName); },
        },
        event);
}
}  // namespace zappy::server::game
