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
#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/render/utils/Rectangle.hpp"
#include "zappy/gui/render/utils/Vector2.hpp"

namespace zappy::gui::ui {

Hud::Hud(network::CommandSender& sender, int initialTimeUnit, int windowWidth, int windowHeight)
    : _windowWidth{windowWidth}, _windowHeight{windowHeight}, _timeSlider{sender, initialTimeUnit} {}

bool Hud::victoryActive(const game::GameState& state) const { return state.winner().has_value() && !_victoryDismissed; }

render::Rectangle Hud::chatPanelAnchor() const {
    return render::Rectangle{kChatPanelMargin, static_cast<float>(_windowHeight) - kChatPanelHeight - kChatPanelMargin,
                             kChatPanelWidth, kChatPanelHeight};
}

render::Rectangle Hud::timeSliderAnchor() {
    return render::Rectangle{kTimeSliderMarginRight, kTimeSliderMarginTop, kTimeSliderWidth, kTimeSliderHeight};
}

render::Rectangle Hud::infoPanelAnchor(const game::GameState& state) const {
    const float infoPanelX = static_cast<float>(_windowWidth) - kInfoPanelWidth - kInfoPanelMargin;
    const float infoPanelMaxHeight = static_cast<float>(_windowHeight) - (2.0F * kInfoPanelMargin);
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

    const float infoPanelX = static_cast<float>(_windowWidth) - kInfoPanelWidth - kInfoPanelMargin;
    const render::Rectangle infoPanelHeaderProbe{infoPanelX, kInfoPanelMargin, kInfoPanelWidth, 0.0F};
    _infoPanel.update(mousePos, clicked, camera, state, infoPanelHeaderProbe);
}

void Hud::draw(const game::GameState& state) {
    const auto& winner = state.winner();
    if (winner.has_value() && !_victoryDismissed) {
        const render::Rectangle screenBounds{0.0F, 0.0F, static_cast<float>(_windowWidth),
                                             static_cast<float>(_windowHeight)};
        if (VictoryScreen::draw(winner.value(), screenBounds)) {
            _victoryDismissed = true;
        }
        return;
    }

    _chatPanel.draw(state.broadcasts(), chatPanelAnchor());
    _timeSlider.draw(timeSliderAnchor());
    _infoPanel.draw(state, infoPanelAnchor(state));
}

std::optional<std::uint32_t> Hud::focusedPlayerId() const {
    return _infoPanel.state() == InfoPanelState::Player ? _infoPanel.selectedPlayerId() : std::nullopt;
}

}  // namespace zappy::gui::ui
