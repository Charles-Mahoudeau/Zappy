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

#include "Widgets.hpp"
#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/render/Camera.hpp"
#include "zappy/gui/ui/utils/Rectangle.hpp"
#include "zappy/gui/ui/utils/Vector2.hpp"

namespace zappy::gui::ui {

std::optional<std::uint32_t> InfoPanel::pickPlayer(Vector2 /*mousePos*/, const render::Camera& /*camera*/,
                                                   const game::GameState& /*state*/) {
    return std::nullopt;
}

std::optional<std::pair<std::uint32_t, std::uint32_t>> InfoPanel::pickTile(Vector2 /*mousePos*/,
                                                                           const render::Camera& /*camera*/,
                                                                           const game::GameState& /*state*/) {
    return std::nullopt;
}

void InfoPanel::update(Vector2 mousePos, bool clicked, const render::Camera& camera, const game::GameState& state) {
    if (const auto playerId = pickPlayer(mousePos, camera, state)) {
        _selectedPlayerId = playerId;
        _state = InfoPanelState::Player;
        return;
    }
    if (const auto tile = pickTile(mousePos, camera, state)) {
        _selectedTile = tile;
        _state = InfoPanelState::Tile;
        return;
    }
    if (clicked) {
        _state = InfoPanelState::Leaderboard;
    }
}

InfoPanelState InfoPanel::state() const { return _state; }

std::optional<std::pair<std::uint32_t, std::uint32_t>> InfoPanel::selectedTile() const { return _selectedTile; }

std::optional<std::uint32_t> InfoPanel::selectedPlayerId() const { return _selectedPlayerId; }

void InfoPanel::draw(const game::GameState& /*state*/, Rectangle bounds) const {
    switch (_state) {
        case InfoPanelState::Leaderboard:
            Widgets::panel(bounds, "Leaderboard");
            break;
        case InfoPanelState::Tile:
            Widgets::panel(bounds, "Tile");
            break;
        case InfoPanelState::Player:
            Widgets::panel(bounds, "Player");
            break;
    }
}

}  // namespace zappy::gui::ui
