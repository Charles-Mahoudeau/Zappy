/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** InfoPanel
*/

#include "InfoPanel.hpp"

#include <cstdint>
#include <optional>
#include <utility>

#include "Leaderboard.hpp"
#include "PlayerInfo.hpp"
#include "TileInfo.hpp"
#include "utils/Rectangle.hpp"
#include "utils/Vector2.hpp"
#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/render/Camera.hpp"

namespace zappy::gui::ui {

std::optional<std::uint32_t> InfoPanel::pickPlayer(Vector2 mousePos, const render::Camera& camera,
                                                   const game::GameState& state) {
    return PlayerInfo::pick(mousePos, camera, state);
}

std::optional<std::pair<std::uint32_t, std::uint32_t>> InfoPanel::pickTile(Vector2 mousePos,
                                                                           const render::Camera& camera,
                                                                           const game::GameState& state) {
    return TileInfo::pick(mousePos, camera, state);
}

void InfoPanel::update(Vector2 mousePos, bool clicked, const render::Camera& camera, const game::GameState& state) {
    using enum InfoPanelState;

    if (_state == Player && _selectedPlayerId.has_value() && !state.players().contains(*_selectedPlayerId)) {
        _selectedPlayerId.reset();
        _state = Leaderboard;
    }

    if (!clicked) {
        return;
    }

    if (const auto playerId = pickPlayer(mousePos, camera, state); playerId.has_value()) {
        _selectedPlayerId = playerId;
        _state = Player;
        return;
    }
    if (const auto tile = pickTile(mousePos, camera, state); tile.has_value()) {
        _selectedTile = tile;
        _state = Tile;
        return;
    }
    _state = Leaderboard;
}

InfoPanelState InfoPanel::state() const { return _state; }

std::optional<std::pair<std::uint32_t, std::uint32_t>> InfoPanel::selectedTile() const { return _selectedTile; }

std::optional<std::uint32_t> InfoPanel::selectedPlayerId() const { return _selectedPlayerId; }

void InfoPanel::draw(const game::GameState& state, Rectangle bounds) const {
    using enum InfoPanelState;

    switch (_state) {
        case Leaderboard:
            Leaderboard::draw(state, bounds);
            break;
        case Tile:
            if (_selectedTile.has_value()) {
                TileInfo::draw(state, _selectedTile.value().first, _selectedTile.value().second, bounds);
            }
            break;
        case Player:
            if (_selectedPlayerId.has_value()) {
                PlayerInfo::draw(state, _selectedPlayerId.value(), bounds);
            }
            break;
    }
}

}  // namespace zappy::gui::ui
