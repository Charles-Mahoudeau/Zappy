/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** EventHelper
*/

#include "zappy/server/game/EventHelper.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <functional>
#include <optional>
#include <string>

#include "zappy/server/game/Event.hpp"
#include "zappy/server/game/Inventory.hpp"
#include "zappy/server/game/ResourceType.hpp"
#include "zappy/shared/math/Direction.hpp"

using zappy::math::Direction;
using zappy::server::game::EventHelper;
using zappy::server::game::Inventory;
using zappy::server::game::ResourceType;

// ─── TileInventoryEvent ──────────────────────────────────────────────────────

TEST(EventHelperToWire, TileInventoryEmptyInventory) {
    const Inventory inventory;
    const zappy::server::game::Event event = zappy::server::game::TileInventoryEvent{
        .position = {3, 7},
        .inventory = std::cref(inventory),
    };

    EXPECT_EQ(EventHelper::toWire(event), "bct 3 7 0 0 0 0 0 0 0");
}

TEST(EventHelperToWire, TileInventoryWithResources) {
    using enum ResourceType;

    Inventory inventory;
    inventory.addResource(kFood, 5);
    inventory.addResource(kLinemate, 2);
    inventory.addResource(kThystame, 1);

    const zappy::server::game::Event event = zappy::server::game::TileInventoryEvent{
        .position = {0, 0},
        .inventory = std::cref(inventory),
    };

    EXPECT_EQ(EventHelper::toWire(event), "bct 0 0 5 2 0 0 0 0 1");
}

// ─── PlayerConnectionEvent ───────────────────────────────────────────────────

TEST(EventHelperToWire, PlayerConnectionNorth) {
    const zappy::server::game::Event event = zappy::server::game::PlayerConnectionEvent{
        .playerId = 1,
        .position = {4, 2},
        .orientation = Direction::kNorth,
        .level = 1,
        .teamName = "alpha",
    };

    // orientation kNorth = 0, wire = 0 + 1 = 1
    EXPECT_EQ(EventHelper::toWire(event), "pnw #1 4 2 1 1 alpha");
}

TEST(EventHelperToWire, PlayerConnectionEast) {
    const zappy::server::game::Event event = zappy::server::game::PlayerConnectionEvent{
        .playerId = 42,
        .position = {0, 0},
        .orientation = Direction::kEast,
        .level = 3,
        .teamName = "beta",
    };

    // orientation kEast = 1, wire = 1 + 1 = 2
    EXPECT_EQ(EventHelper::toWire(event), "pnw #42 0 0 2 3 beta");
}

TEST(EventHelperToWire, PlayerConnectionSouth) {
    const zappy::server::game::Event event = zappy::server::game::PlayerConnectionEvent{
        .playerId = 7,
        .position = {10, 5},
        .orientation = Direction::kSouth,
        .level = 8,
        .teamName = "team1",
    };

    // orientation kSouth = 2, wire = 2 + 1 = 3
    EXPECT_EQ(EventHelper::toWire(event), "pnw #7 10 5 3 8 team1");
}

TEST(EventHelperToWire, PlayerConnectionWest) {
    const zappy::server::game::Event event = zappy::server::game::PlayerConnectionEvent{
        .playerId = 99,
        .position = {1, 1},
        .orientation = Direction::kWest,
        .level = 2,
        .teamName = "gamma",
    };

    // orientation kWest = 3, wire = 3 + 1 = 4
    EXPECT_EQ(EventHelper::toWire(event), "pnw #99 1 1 4 2 gamma");
}

// ─── PlayerPositionEvent ─────────────────────────────────────────────────────

TEST(EventHelperToWire, PlayerPosition) {
    constexpr zappy::server::game::Event event = zappy::server::game::PlayerPositionEvent{
        .playerId = 5,
        .position = {8, 3},
        .orientation = Direction::kSouth,
    };

    // kSouth = 2, wire = 3
    EXPECT_EQ(EventHelper::toWire(event), "ppo #5 8 3 3");
}

TEST(EventHelperToWire, PlayerPositionOriginNorth) {
    constexpr zappy::server::game::Event event = zappy::server::game::PlayerPositionEvent{
        .playerId = 0,
        .position = {0, 0},
        .orientation = Direction::kNorth,
    };

    EXPECT_EQ(EventHelper::toWire(event), "ppo #0 0 0 1");
}

// ─── PlayerLevelEvent ────────────────────────────────────────────────────────

TEST(EventHelperToWire, PlayerLevel) {
    constexpr zappy::server::game::Event event = zappy::server::game::PlayerLevelEvent{
        .playerId = 12,
        .level = 4,
    };

    EXPECT_EQ(EventHelper::toWire(event), "plv #12 4");
}

TEST(EventHelperToWire, PlayerLevelMax) {
    constexpr zappy::server::game::Event event = zappy::server::game::PlayerLevelEvent{
        .playerId = 0,
        .level = 8,
    };

    EXPECT_EQ(EventHelper::toWire(event), "plv #0 8");
}

// ─── PlayerInventoryEvent ────────────────────────────────────────────────────

TEST(EventHelperToWire, PlayerInventoryEmpty) {
    const Inventory inventory;
    const zappy::server::game::Event event = zappy::server::game::PlayerInventoryEvent{
        .playerId = 3,
        .position = {2, 6},
        .inventory = std::cref(inventory),
    };

    EXPECT_EQ(EventHelper::toWire(event), "pin #3 2 6 0 0 0 0 0 0 0");
}

TEST(EventHelperToWire, PlayerInventoryWithResources) {
    Inventory inventory;
    inventory.addResource(ResourceType::kFood, 10);
    inventory.addResource(ResourceType::kDeraumere, 3);

    const zappy::server::game::Event event = zappy::server::game::PlayerInventoryEvent{
        .playerId = 17,
        .position = {5, 9},
        .inventory = std::cref(inventory),
    };

    EXPECT_EQ(EventHelper::toWire(event), "pin #17 5 9 10 0 3 0 0 0 0");
}

// ─── PlayerExpulsionEvent ────────────────────────────────────────────────────

TEST(EventHelperToWire, PlayerExpulsion) {
    constexpr zappy::server::game::Event event = zappy::server::game::PlayerExpulsionEvent{
        .playerId = 21,
    };

    EXPECT_EQ(EventHelper::toWire(event), "pex #21");
}

// ─── PlayerBroadcastEvent ────────────────────────────────────────────────────

TEST(EventHelperToWire, PlayerBroadcast) {
    const zappy::server::game::Event event = zappy::server::game::PlayerBroadcastEvent{
        .playerId = 8,
        .message = "hello world",
    };

    // NOTE: the implementation currently formats this as "pex #id message"
    EXPECT_EQ(EventHelper::toWire(event), "pbc #8 hello world");
}

TEST(EventHelperToWire, PlayerBroadcastEmptyMessage) {
    const zappy::server::game::Event event = zappy::server::game::PlayerBroadcastEvent{
        .playerId = 0,
        .message = "",
    };

    EXPECT_EQ(EventHelper::toWire(event), "pbc #0 ");
}

// ─── IncantationBeginEvent ───────────────────────────────────────────────────

TEST(EventHelperToWire, IncantationBeginSinglePlayer) {
    const zappy::server::game::Event event = zappy::server::game::IncantationBeginEvent{
        .position = {4, 4},
        .level = 2,
        .playerIds = {10},
    };

    EXPECT_EQ(EventHelper::toWire(event), "pic 4 4 2 #10");
}

TEST(EventHelperToWire, IncantationBeginMultiplePlayers) {
    const zappy::server::game::Event event = zappy::server::game::IncantationBeginEvent{
        .position = {1, 2},
        .level = 5,
        .playerIds = {3, 7, 11},
    };

    EXPECT_EQ(EventHelper::toWire(event), "pic 1 2 5 #3 #7 #11");
}

TEST(EventHelperToWire, IncantationBeginNoPlayers) {
    constexpr zappy::server::game::Event event = zappy::server::game::IncantationBeginEvent{
        .position = {0, 0},
        .level = 1,
        .playerIds = {},
    };

    EXPECT_EQ(EventHelper::toWire(event), "pic 0 0 1");
}

// ─── IncantationEndEvent ─────────────────────────────────────────────────────

TEST(EventHelperToWire, IncantationEndSuccess) {
    constexpr zappy::server::game::Event event = zappy::server::game::IncantationEndEvent{
        .position = {6, 3},
        .success = true,
    };

    EXPECT_EQ(EventHelper::toWire(event), "pie 6 3 1");
}

TEST(EventHelperToWire, IncantationEndFailure) {
    constexpr zappy::server::game::Event event = zappy::server::game::IncantationEndEvent{
        .position = {0, 0},
        .success = false,
    };

    EXPECT_EQ(EventHelper::toWire(event), "pie 0 0 0");
}

// ─── PlayerEggLayingEvent ────────────────────────────────────────────────────

TEST(EventHelperToWire, PlayerEggLaying) {
    constexpr zappy::server::game::Event event = zappy::server::game::PlayerEggLayingEvent{
        .playerId = 55,
    };

    EXPECT_EQ(EventHelper::toWire(event), "pfk #55");
}

// ─── PlayerResourceDropEvent ─────────────────────────────────────────────────

TEST(EventHelperToWire, PlayerResourceDropFood) {
    constexpr zappy::server::game::Event event = zappy::server::game::PlayerResourceDropEvent{
        .playerId = 2,
        .resourceType = ResourceType::kFood,
    };

    // kFood = 0
    EXPECT_EQ(EventHelper::toWire(event), "pdr #2 0");
}

TEST(EventHelperToWire, PlayerResourceDropThystame) {
    constexpr zappy::server::game::Event event = zappy::server::game::PlayerResourceDropEvent{
        .playerId = 9,
        .resourceType = ResourceType::kThystame,
    };

    // kThystame = 6
    EXPECT_EQ(EventHelper::toWire(event), "pdr #9 6");
}

// ─── PlayerResourceCollectEvent ──────────────────────────────────────────────

TEST(EventHelperToWire, PlayerResourceCollectLinemate) {
    constexpr zappy::server::game::Event event = zappy::server::game::PlayerResourceCollectEvent{
        .playerId = 14,
        .resourceType = ResourceType::kLinemate,
    };

    // kLinemate = 1
    EXPECT_EQ(EventHelper::toWire(event), "pgt #14 1");
}

TEST(EventHelperToWire, PlayerResourceCollectPhiras) {
    constexpr zappy::server::game::Event event = zappy::server::game::PlayerResourceCollectEvent{
        .playerId = 0,
        .resourceType = ResourceType::kPhiras,
    };

    // kPhiras = 5
    EXPECT_EQ(EventHelper::toWire(event), "pgt #0 5");
}

// ─── PlayerDeathEvent ────────────────────────────────────────────────────────

TEST(EventHelperToWire, PlayerDeath) {
    constexpr zappy::server::game::Event event = zappy::server::game::PlayerDeathEvent{
        .playerId = 33,
    };

    EXPECT_EQ(EventHelper::toWire(event), "pdi #33");
}

// ─── EggLaidEvent ────────────────────────────────────────────────────────────

TEST(EventHelperToWire, EggLaidWithPlayer) {
    constexpr zappy::server::game::Event event = zappy::server::game::EggLaidEvent{
        .playerId = std::optional<std::uint64_t>{4},
        .eggId = 100,
        .position = {2, 8},
    };

    EXPECT_EQ(EventHelper::toWire(event), "enw #100 #4 2 8");
}

TEST(EventHelperToWire, EggLaidWithoutPlayer) {
    constexpr zappy::server::game::Event event = zappy::server::game::EggLaidEvent{
        .playerId = std::nullopt,
        .eggId = 200,
        .position = {0, 0},
    };

    // value_or(-1) casts std::nullopt -> -1
    EXPECT_EQ(EventHelper::toWire(event), "enw #200 #-1 0 0");
}

// ─── EggConnectionEvent ──────────────────────────────────────────────────────

TEST(EventHelperToWire, EggConnection) {
    constexpr zappy::server::game::Event event = zappy::server::game::EggConnectionEvent{
        .eggId = 77,
    };

    EXPECT_EQ(EventHelper::toWire(event), "ebo #77");
}

// ─── EggDeathEvent ───────────────────────────────────────────────────────────

TEST(EventHelperToWire, EggDeath) {
    constexpr zappy::server::game::Event event = zappy::server::game::EggDeathEvent{
        .eggId = 88,
    };

    EXPECT_EQ(EventHelper::toWire(event), "edi #88");
}

// ─── GameEndEvent ────────────────────────────────────────────────────────────

TEST(EventHelperToWire, GameEnd) {
    const zappy::server::game::Event event = zappy::server::game::GameEndEvent{
        .teamName = "winners",
    };

    EXPECT_EQ(EventHelper::toWire(event), "seg winners");
}
