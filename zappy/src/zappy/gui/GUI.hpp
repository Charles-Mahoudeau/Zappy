/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** GUI
*/

#pragma once

#include <string_view>

#include "zappy/gui/GuiCliParser.hpp"
#include "zappy/gui/display/Window.hpp"
#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/network/CommandSender.hpp"
#include "zappy/gui/network/Handshake.hpp"
#include "zappy/gui/network/ProtocolParser.hpp"
#include "zappy/gui/render/AssetStore.hpp"
#include "zappy/gui/render/Camera.hpp"
#include "zappy/gui/render/MusicManager.hpp"
#include "zappy/gui/render/Renderer.hpp"
#include "zappy/gui/ui/DotAnimator.hpp"
#include "zappy/gui/ui/GuiTheme.hpp"
#include "zappy/gui/ui/Hud.hpp"
#include "zappy/shared/io/Poller.hpp"
#include "zappy/shared/network/Address.hpp"
#include "zappy/shared/network/BufferedClient.hpp"

namespace zappy::gui {

class GUI {
  public:
    GUI();
    ~GUI() = default;

    GUI(const GUI&) = delete;
    GUI& operator=(const GUI&) = delete;

    GUI(GUI&&) noexcept = default;
    GUI& operator=(GUI&&) noexcept = default;

    int init(const GuiCliParser& cli);
    int run();

    void connect(const GuiCliParser& cli);
    void pump();

    [[nodiscard]] const game::GameState& state() const;

  private:
    void drawLoadingFrame(std::string_view name, float progress);
    void drawWaitingFrame();
    bool tryConnect();
    bool waitForConnection();
    void runSession();

    zappy::network::Address _address;
    zappy::network::BufferedClient _buffer;
    game::GameState _state;
    network::ProtocolParser _parser;
    network::CommandSender _sender;
    network::Handshake _handshake;
    zappy::io::Poller _poller;

    display::Window _window;
    ui::GuiTheme _theme;
    render::Camera _camera;
    render::AssetStore _assets;
    render::Renderer _renderer;
    render::MusicManager _musicManager;
    ui::Hud _hud;
    ui::DotAnimator _dotAnimator;
};

}  // namespace zappy::gui
