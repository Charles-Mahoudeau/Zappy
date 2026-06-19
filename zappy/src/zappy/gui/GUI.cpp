/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** GUI
*/

#include "zappy/gui/GUI.hpp"

#include <raylib.h>

#include <cstddef>
#include <string>

#include "zappy/gui/GuiCliParser.hpp"
#include "zappy/gui/display/Window.hpp"
#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/render/Camera.hpp"
#include "zappy/shared/exception/InvalidState.hpp"

namespace zappy::gui {

static constexpr int kWindowWidth = 1600;
static constexpr int kWindowHeight = 900;
static constexpr int kTargetFPS = 60;
static constexpr int kPollTimeoutMs = 16;

GUI::GUI()
    : _parser{_state},
      _sender{_buffer},
      _handshake{_buffer, _sender, _parser, _state},
      _camera{
          Vector3{.x = 0, .y = 0, .z = 10},
          Vector3{.x = 0, .y = 0, .z = 0},
          Vector3{.x = 0, .y = 1, .z = 0},
          45.0F,
          render::CameraProjection::CAMERA_ORTHOGRAPHIC,
      } {}

void GUI::connect(const GuiCliParser& cli) {
    _address = zappy::network::Address{std::string{cli.host()}, cli.port()};

    _buffer.connect(_address);
    _handshake.run();
}

const game::GameState& GUI::state() const { return _state; }

void GUI::pump() {
    _buffer.poll();
    while (_buffer.hasMessages()) {
        _parser.dispatch(_buffer.popMessage());
    }
}

int GUI::run(const GuiCliParser& cli) {
    connect(cli);

    display::Window window{kWindowWidth, kWindowHeight, "Zappy"};
    window.setTargetFPS(kTargetFPS);

    _poller.add(_buffer.fd(), zappy::io::Poller::kPollRead | zappy::io::Poller::kPollError, [this](std::byte events) {
        if ((events & zappy::io::Poller::kPollError) != zappy::io::Poller::kPollNone) {
            throw exception::InvalidState{"server disconnected"};
        }
        pump();
    });

    while (!window.shouldClose()) {
        _poller.poll(kPollTimeoutMs);
        window.beginFrame();
        window.endFrame();
    }
    return 0;
}

}  // namespace zappy::gui
