/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** GameState
*/

#include "GameState.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "zappy/gui/render/utils/Vector3.hpp"
#include "zappy/shared/exception/InvalidArgument.hpp"

namespace zappy::gui::game {

void GameState::setMapSize(std::size_t width, std::size_t height) {
    _width = width;
    _height = height;
    _tiles.assign(width * height, Resources{});
    _tileSet.assign(width * height, false);
    _tilesReceived = 0;
}

void GameState::setTile(std::size_t x, std::size_t y, const Resources& resources) {
    if (x >= _width || y >= _height) {
        throw exception::InvalidArgument{"Tile coordinates out of bounds"};
    }
    const std::size_t idx = (y * _width) + x;
    if (!_tileSet.at(idx)) {
        _tileSet.at(idx) = true;
        ++_tilesReceived;
    }
    _tiles.at(idx) = resources;
}

void GameState::addTeam(std::string name) {
    if (std::ranges::find(_teams, name) != _teams.end()) {
        return;
    }
    _teams.push_back(std::move(name));
}

void GameState::addPlayer(std::uint32_t id, std::uint32_t x, std::uint32_t y, Orientation orientation,
                          std::uint32_t level, std::string team) {
    _players.try_emplace(id,
                         Player{.x = x, .y = y, .orientation = orientation, .level = level, .team = std::move(team)});
}

void GameState::setPlayerPosition(std::uint32_t id, std::uint32_t x, std::uint32_t y, Orientation orientation) {
    auto it = _players.find(id);
    if (it == _players.end()) {
        throw exception::InvalidArgument{"Unknown player id"};
    }
    it->second.x = x;
    it->second.y = y;
    it->second.orientation = orientation;
}

void GameState::setPlayerLevel(std::uint32_t id, std::uint32_t level) {
    auto it = _players.find(id);
    if (it == _players.end()) {
        throw exception::InvalidArgument{"Unknown player id"};
    }
    it->second.level = level;
}

void GameState::setPlayerInventory(std::uint32_t id, std::uint32_t x, std::uint32_t y, const Resources& inventory) {
    auto it = _players.find(id);
    if (it == _players.end()) {
        throw exception::InvalidArgument{"Unknown player id"};
    }
    it->second.x = x;
    it->second.y = y;
    it->second.inventory = inventory;
}

void GameState::removePlayer(std::uint32_t id) { _players.erase(id); }

void GameState::addEgg(std::uint32_t eggId, std::int32_t playerId, std::uint32_t x, std::uint32_t y) {
    _eggs.emplace(eggId, Egg{.playerId = playerId, .x = x, .y = y});
}

void GameState::removeEgg(std::uint32_t eggId) { _eggs.erase(eggId); }

void GameState::setTimeUnit(std::uint32_t timeUnit) { _timeUnit = timeUnit; }

void GameState::setWinner(std::string team) { _winner = std::move(team); }

void GameState::addBroadcast(std::string message) {
    if (_broadcasts.size() == 10) {
        _broadcasts.pop_front();
    }
    _broadcasts.push_back(std::move(message));
}

void GameState::setPlayerIncanting(std::uint32_t id, bool incanting) {
    auto it = _players.find(id);
    if (it == _players.end()) {
        throw exception::InvalidArgument{"Unknown player id"};
    }
    it->second.isIncanting = incanting;
}

std::size_t GameState::width() const { return _width; }

std::size_t GameState::height() const { return _height; }

const std::vector<Resources>& GameState::tiles() const { return _tiles; }

const Resources& GameState::tile(std::size_t x, std::size_t y) const {
    if (x >= _width || y >= _height) {
        throw exception::InvalidArgument{"Tile coordinates out of bounds"};
    }
    return _tiles.at((y * _width) + x);
}

const std::unordered_map<std::uint32_t, Player>& GameState::players() const { return _players; }

const std::optional<Player> GameState::getPlayer(std::uint32_t id) const {
    auto it = _players.find(id);
    if (it == _players.end()) {
        return std::nullopt;
    }
    return it->second;
}

const std::vector<std::string>& GameState::teams() const { return _teams; }

std::size_t GameState::playersAtMaxLevel(const std::string& team) const {
    return static_cast<std::size_t>(std::ranges::count_if(
        _players, [&team](const auto& entry) { return entry.second.team == team && entry.second.level == kMaxLevel; }));
}

const std::unordered_map<std::uint32_t, Egg>& GameState::eggs() const { return _eggs; }

std::optional<zappy::gui::game::Egg> GameState::getEgg(std::uint32_t eggId) const {
    auto it = _eggs.find(eggId);
    if (it != _eggs.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::uint32_t GameState::timeUnit() const { return _timeUnit; }

bool GameState::isGameOver() const { return _winner.has_value(); }

const std::optional<std::string>& GameState::winner() const { return _winner; }

bool GameState::isReady() const { return _width > 0 && _height > 0 && _tilesReceived >= _width * _height; }

const std::deque<std::string>& GameState::broadcasts() const { return _broadcasts; }

void GameState::broadcastEvent(EventType type, render::Vector3 position) {
    _event.type = type;
    _event.position = position;
}
[[nodiscard]] Event& GameState::getEvent() { return _event; }

}  // namespace zappy::gui::game
