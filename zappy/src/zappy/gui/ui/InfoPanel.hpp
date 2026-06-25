/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** InfoPanel
*/

#pragma once

#include <cstdint>
#include <optional>
#include <utility>

#include "Draggable.hpp"
#include "zappy/gui/render/utils/Rectangle.hpp"
#include "zappy/gui/render/utils/Vector2.hpp"

namespace zappy::gui::render {
class Camera;
}

namespace zappy::gui::game {
class GameState;
}

namespace zappy::gui::ui {

enum class InfoPanelState : std::uint8_t { Leaderboard, Tile, Player };

class InfoPanel {
  public:
    InfoPanel() = default;
    ~InfoPanel() = default;

    InfoPanel(const InfoPanel&) = delete;
    InfoPanel& operator=(const InfoPanel&) = delete;

    InfoPanel(InfoPanel&&) noexcept = default;
    InfoPanel& operator=(InfoPanel&&) noexcept = default;

    void update(render::Vector2 mousePos, bool clicked, const render::Camera& camera, const game::GameState& state,
                render::Rectangle panelBounds = render::Rectangle{});
    void draw(const game::GameState& state, render::Rectangle bounds);

    [[nodiscard]] float contentHeight(const game::GameState& state) const;

    [[nodiscard]] InfoPanelState state() const;
    [[nodiscard]] std::optional<std::pair<std::uint32_t, std::uint32_t>> selectedTile() const;
    [[nodiscard]] std::optional<std::uint32_t> selectedPlayerId() const;

  private:
    [[nodiscard]] static std::optional<std::uint32_t> pickPlayer(render::Vector2 mousePos, const render::Camera& camera,
                                                                 const game::GameState& state);
    [[nodiscard]] static std::optional<std::pair<std::uint32_t, std::uint32_t>> pickTile(render::Vector2 mousePos,
                                                                                         const render::Camera& camera,
                                                                                         const game::GameState& state);

    InfoPanelState _state{InfoPanelState::Leaderboard};
    std::optional<std::pair<std::uint32_t, std::uint32_t>> _selectedTile;
    std::optional<std::uint32_t> _selectedPlayerId;
    Draggable _drag;
};

}  // namespace zappy::gui::ui
