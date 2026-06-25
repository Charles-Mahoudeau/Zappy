/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Hud
*/

#pragma once

#include <cstdint>
#include <optional>

#include "ChatPanel.hpp"
#include "InfoPanel.hpp"
#include "TimeUnitSlider.hpp"
#include "zappy/gui/render/utils/Rectangle.hpp"
#include "zappy/gui/render/utils/Vector2.hpp"

namespace zappy::gui::render {
class Camera;
}

namespace zappy::gui::game {
class GameState;
}

namespace zappy::gui::network {
class CommandSender;
}

namespace zappy::gui::ui {

class Hud {
  public:
    Hud(network::CommandSender& sender, int initialTimeUnit, int windowWidth, int windowHeight);
    ~Hud() = default;

    Hud(const Hud&) = delete;
    Hud& operator=(const Hud&) = delete;

    Hud(Hud&&) noexcept = default;
    Hud& operator=(Hud&&) noexcept = default;

    void update(const render::Camera& camera, const game::GameState& state);
    void draw(const game::GameState& state);

    [[nodiscard]] std::optional<std::uint32_t> focusedPlayerId() const;

  private:
    [[nodiscard]] bool victoryActive(const game::GameState& state) const;
    [[nodiscard]] bool isOverAnyPanel(render::Vector2 mousePos, const game::GameState& state) const;
    [[nodiscard]] render::Rectangle chatPanelAnchor() const;
    [[nodiscard]] static render::Rectangle timeSliderAnchor();
    [[nodiscard]] render::Rectangle infoPanelAnchor(const game::GameState& state) const;

    static constexpr float kChatPanelWidth = 320.0F;
    static constexpr float kChatPanelHeight = 180.0F;
    static constexpr float kChatPanelMargin = 8.0F;
    static constexpr float kTimeSliderWidth = 220.0F;
    static constexpr float kTimeSliderHeight = 24.0F;
    static constexpr float kTimeSliderMarginTop = 8.0F;
    static constexpr float kTimeSliderMarginRight = 70.0F;
    static constexpr float kInfoPanelWidth = 320.0F;
    static constexpr float kInfoPanelMargin = 8.0F;

    int _windowWidth;
    int _windowHeight;
    ChatPanel _chatPanel;
    TimeUnitSlider _timeSlider;
    InfoPanel _infoPanel;
    bool _victoryDismissed{false};
};

}  // namespace zappy::gui::ui
