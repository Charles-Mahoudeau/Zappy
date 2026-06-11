/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** GameState
*/

#include "GameState.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "zappy/shared/exception/InvalidArgument.hpp"

namespace zappy::gui::game {

void GameState::setMapSize(std::uint32_t width, std::uint32_t height) {
    _width = width;
    _height = height;
    _tiles.assign(height, std::vector<Resources>(width));
    _tilesReceived = 0;
}

void GameState::setTile(std::uint32_t x, std::uint32_t y, const Resources& resources) {
    if (x >= _width || y >= _height) {
        throw exception::InvalidArgument{"Tile coordinates out of bounds"};
    }
    _tiles.at(y).at(x) = resources;
    ++_tilesReceived;
}

void GameState::addTeam(std::string name) { _teams.push_back(std::move(name)); }

void GameState::addPlayer(std::uint32_t id, std::uint32_t x, std::uint32_t y, Orientation orientation,
                          std::uint32_t level, std::string team) {
    _players.emplace(id, Player{.x = x, .y = y, .orientation = orientation, .level = level, .team = std::move(team)});
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

void GameState::addEgg(std::uint32_t eggId, std::uint32_t playerId, std::uint32_t x, std::uint32_t y) {
    _eggs.emplace(eggId, Egg{.playerId = playerId, .x = x, .y = y});
}

void GameState::removeEgg(std::uint32_t eggId) { _eggs.erase(eggId); }

void GameState::setTimeUnit(std::uint32_t timeUnit) { _timeUnit = timeUnit; }

void GameState::setWinner(std::string team) { _winner = std::move(team); }

std::uint32_t GameState::width() const { return _width; }

std::uint32_t GameState::height() const { return _height; }

const std::vector<std::vector<Resources>>& GameState::tiles() const { return _tiles; }

const std::unordered_map<std::uint32_t, Player>& GameState::players() const { return _players; }

const std::vector<std::string>& GameState::teams() const { return _teams; }

const std::unordered_map<std::uint32_t, Egg>& GameState::eggs() const { return _eggs; }

std::uint32_t GameState::timeUnit() const { return _timeUnit; }

bool GameState::isGameOver() const { return _winner.has_value(); }

const std::optional<std::string>& GameState::winner() const { return _winner; }

bool GameState::isReady() const { return _width > 0 && _height > 0 && _tilesReceived >= _width * _height; }

}  // namespace zappy::gui::game
