/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** GameState
*/

#pragma once

#include <cstddef>
#include <cstdint>
#include <deque>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "EventHandler.hpp"
#include "zappy/gui/render/utils/Vector3.hpp"

namespace zappy::gui::game {

enum class ResourceType : std::uint8_t {
    Food = 0,
    Linemate = 1,
    Deraumere = 2,
    Sibur = 3,
    Mendiane = 4,
    Phiras = 5,
    Thystame = 6
};

struct Resources {
    std::uint32_t food{0};
    std::uint32_t linemate{0};
    std::uint32_t deraumere{0};
    std::uint32_t sibur{0};
    std::uint32_t mendiane{0};
    std::uint32_t phiras{0};
    std::uint32_t thystame{0};
};

enum class Orientation : std::uint8_t { North = 1, East = 2, South = 3, West = 4 };

struct Player {
    std::uint32_t x{0};
    std::uint32_t y{0};
    Orientation orientation{Orientation::North};
    std::uint32_t level{1};
    std::string team;
    Resources inventory;
    bool isIncanting{false};
};

struct Egg {
    std::int32_t playerId{0};
    std::uint32_t x{0};
    std::uint32_t y{0};
};

class GameState {
  public:
    GameState() = default;
    ~GameState() = default;

    GameState(const GameState&) = delete;
    GameState& operator=(const GameState&) = delete;

    GameState(GameState&&) noexcept = default;
    GameState& operator=(GameState&&) noexcept = default;

    void setMapSize(std::size_t width, std::size_t height);
    void setTile(std::size_t x, std::size_t y, const Resources& resources);
    void addTeam(std::string name);
    void addPlayer(std::uint32_t id, std::uint32_t x, std::uint32_t y, Orientation orientation, std::uint32_t level,
                   std::string team);
    void setPlayerPosition(std::uint32_t id, std::uint32_t x, std::uint32_t y, Orientation orientation);
    void setPlayerLevel(std::uint32_t id, std::uint32_t level);
    void setPlayerInventory(std::uint32_t id, std::uint32_t x, std::uint32_t y, const Resources& inventory);
    void removePlayer(std::uint32_t id);
    void addEgg(std::uint32_t eggId, std::int32_t playerId, std::uint32_t x, std::uint32_t y);
    void removeEgg(std::uint32_t eggId);
    void setTimeUnit(std::uint32_t timeUnit);
    void setWinner(std::string team);
    void addBroadcast(std::string message);
    void setPlayerIncanting(std::uint32_t id, bool incanting);

    static constexpr std::uint32_t kMaxLevel = 8;
    static constexpr std::size_t kPlayersNeededToWin = 6;

    [[nodiscard]] std::size_t width() const;
    [[nodiscard]] std::size_t height() const;
    [[nodiscard]] const std::vector<Resources>& tiles() const;
    [[nodiscard]] const Resources& tile(std::size_t x, std::size_t y) const;
    [[nodiscard]] const std::unordered_map<std::uint32_t, Player>& players() const;
    [[nodiscard]] const std::optional<Player> getPlayer(std::uint32_t id) const;
    [[nodiscard]] const std::vector<std::string>& teams() const;
    [[nodiscard]] std::size_t playersAtMaxLevel(const std::string& team) const;
    [[nodiscard]] const std::unordered_map<std::uint32_t, Egg>& eggs() const;
    [[nodiscard]] std::optional<Egg> getEgg(std::uint32_t eggId) const;
    [[nodiscard]] std::uint32_t timeUnit() const;
    [[nodiscard]] bool isGameOver() const;
    [[nodiscard]] const std::optional<std::string>& winner() const;
    [[nodiscard]] bool isReady() const;
    [[nodiscard]] const std::deque<std::string>& broadcasts() const;

    void broadcastEvent(EventType type, render::Vector3 position = {0.0F, 0.0F, 0.0F});
    [[nodiscard]] Event& getEvent();

  private:
    std::size_t _width{0};
    std::size_t _height{0};
    std::vector<Resources> _tiles;
    std::unordered_map<std::uint32_t, Player> _players;
    std::vector<std::string> _teams;
    std::unordered_map<std::uint32_t, Egg> _eggs;
    std::uint32_t _timeUnit{0};
    std::optional<std::string> _winner;
    std::size_t _tilesReceived{0};
    std::vector<bool> _tileSet;
    std::deque<std::string> _broadcasts;
    Event _event{};
};

}  // namespace zappy::gui::game
