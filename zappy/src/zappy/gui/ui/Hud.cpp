/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Hud
*/

#include "Hud.hpp"

#include <algorithm>
#include <cstdint>
#include <optional>

#include "InfoPanel.hpp"
#include "Mouse.hpp"
#include "VictoryScreen.hpp"
#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/render/utils/Rectangle.hpp"

namespace zappy::gui::ui {

Hud::Hud(network::CommandSender& sender, int initialTimeUnit, int windowWidth, int windowHeight)
    : _windowWidth{windowWidth}, _windowHeight{windowHeight}, _timeSlider{sender, initialTimeUnit} {}

bool Hud::victoryActive(const game::GameState& state) const { return state.winner().has_value() && !_victoryDismissed; }

void Hud::update(const render::Camera& camera, const game::GameState& state) {
    if (victoryActive(state)) {
        return;
    }
    const float infoPanelX = static_cast<float>(_windowWidth) - kInfoPanelWidth - kInfoPanelMargin;
    const render::Rectangle infoPanelHeaderProbe{infoPanelX, kInfoPanelMargin, kInfoPanelWidth, 0.0F};
    _infoPanel.update(Mouse::position(), Mouse::isLeftButtonPressed(), camera, state, infoPanelHeaderProbe);
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

    _chatPanel.draw(
        state.broadcasts(),
        render::Rectangle{kChatPanelMargin, static_cast<float>(_windowHeight) - kChatPanelHeight - kChatPanelMargin,
                          kChatPanelWidth, kChatPanelHeight});
    _timeSlider.draw(
        render::Rectangle{kTimeSliderMarginRight, kTimeSliderMarginTop, kTimeSliderWidth, kTimeSliderHeight});

    const float infoPanelX = static_cast<float>(_windowWidth) - kInfoPanelWidth - kInfoPanelMargin;
    const float infoPanelMaxHeight = static_cast<float>(_windowHeight) - (2.0F * kInfoPanelMargin);
    const float infoPanelHeight = std::min(_infoPanel.contentHeight(state), infoPanelMaxHeight);
    _infoPanel.draw(state, render::Rectangle{infoPanelX, kInfoPanelMargin, kInfoPanelWidth, infoPanelHeight});
}

std::optional<std::uint32_t> Hud::focusedPlayerId() const {
    return _infoPanel.state() == InfoPanelState::Player ? _infoPanel.selectedPlayerId() : std::nullopt;
}

}  // namespace zappy::gui::ui
