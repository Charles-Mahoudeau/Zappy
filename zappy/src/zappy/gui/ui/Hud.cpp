/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Hud
*/

#include "Hud.hpp"

#include <raylib.h>

#include <algorithm>
#include <cstdint>
#include <optional>

#include "InfoPanel.hpp"
#include "Mouse.hpp"
#include "VictoryScreen.hpp"
#include "zappy/gui/display/Window.hpp"
#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/render/Grid.hpp"
#include "zappy/gui/render/utils/Rectangle.hpp"
#include "zappy/gui/render/utils/Vector2.hpp"
#include "zappy/gui/render/utils/Vector3.hpp"

namespace zappy::gui::ui {

Hud::Hud(network::CommandSender& sender, int initialTimeUnit) : _timeSlider{sender, initialTimeUnit} {}

bool Hud::victoryActive(const game::GameState& state) const { return state.winner().has_value() && !_victoryDismissed; }

render::Rectangle Hud::chatPanelAnchor() {
    return render::Rectangle{kChatPanelMargin,
                             static_cast<float>(display::Window::height()) - kChatPanelHeight - kChatPanelMargin,
                             kChatPanelWidth, kChatPanelHeight};
}

render::Rectangle Hud::timeSliderAnchor() {
    return render::Rectangle{kTimeSliderMarginRight, kTimeSliderMarginTop, kTimeSliderWidth, kTimeSliderHeight};
}

render::Rectangle Hud::infoPanelAnchor(const game::GameState& state) const {
    const float infoPanelX = static_cast<float>(display::Window::width()) - kInfoPanelWidth - kInfoPanelMargin;
    const float infoPanelMaxHeight = static_cast<float>(display::Window::height()) - (2.0F * kInfoPanelMargin);
    const float infoPanelHeight = std::min(_infoPanel.contentHeight(state), infoPanelMaxHeight);
    return render::Rectangle{infoPanelX, kInfoPanelMargin, kInfoPanelWidth, infoPanelHeight};
}

bool Hud::isOverAnyPanel(render::Vector2 mousePos, const game::GameState& state) const {
    const render::Rectangle chatBounds = _chatPanel.currentBounds(chatPanelAnchor());
    const render::Rectangle infoBounds = _infoPanel.currentBounds(infoPanelAnchor(state));
    return CheckCollisionPointRec(mousePos, chatBounds) || CheckCollisionPointRec(mousePos, timeSliderAnchor()) ||
           CheckCollisionPointRec(mousePos, infoBounds);
}

void Hud::update(const render::Camera& camera, const game::GameState& state) {
    if (victoryActive(state)) {
        return;
    }
    const render::Vector2 mousePos = Mouse::position();
    const bool clicked = Mouse::isLeftButtonPressed() && !isOverAnyPanel(mousePos, state);

    const float infoPanelX = static_cast<float>(display::Window::width()) - kInfoPanelWidth - kInfoPanelMargin;
    const render::Rectangle infoPanelHeaderProbe{infoPanelX, kInfoPanelMargin, kInfoPanelWidth, 0.0F};
    _infoPanel.update(mousePos, clicked, camera, state, infoPanelHeaderProbe);
}

void Hud::draw(const game::GameState& state) {
    const auto& winner = state.winner();
    if (winner.has_value() && !_victoryDismissed) {
        const render::Rectangle screenBounds{0.0F, 0.0F, static_cast<float>(display::Window::width()),
                                             static_cast<float>(display::Window::height())};
        if (VictoryScreen::draw(winner.value(), screenBounds)) {
            _victoryDismissed = true;
        }
        return;
    }

    _chatPanel.draw(state.broadcasts(), chatPanelAnchor());
    _timeSlider.draw(state, timeSliderAnchor());
    _infoPanel.draw(state, infoPanelAnchor(state));
}

std::optional<std::uint32_t> Hud::focusedPlayerId() const {
    return _infoPanel.state() == InfoPanelState::Player ? _infoPanel.selectedPlayerId() : std::nullopt;
}

bool Hud::isMouseOverChatPanel() const {
    return CheckCollisionPointRec(Mouse::position(), _chatPanel.currentBounds(chatPanelAnchor()));
}

std::optional<render::Vector3> Hud::focusedPlayerWorldPosition(const game::GameState& state) const {
    const auto playerId = focusedPlayerId();
    if (!playerId.has_value()) {
        return std::nullopt;
    }
    const auto it = state.players().find(*playerId);
    if (it == state.players().end()) {
        return std::nullopt;
    }
    return render::Grid::tileToWorld(static_cast<float>(it->second.x), static_cast<float>(it->second.y));
}

}  // namespace zappy::gui::ui
