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
#include "zappy/gui/render/Camera.hpp"
#include "zappy/shared/exception/InvalidState.hpp"

namespace zappy::gui {

static constexpr int kWindowWidth = 1600;
static constexpr int kWindowHeight = 900;
static constexpr int kTargetFPS = 60;
static constexpr int kPollTimeoutMs = 1000;

GUI::GUI()
    : _parser{_state},
      _sender{_buffer},
      _handshake{_buffer, _sender, _parser, _state},
      _window{kWindowWidth, kWindowHeight, "Zappy"},
      _camera{
          Vector3{.x = 0, .y = 0, .z = 10},
          Vector3{.x = 0, .y = 0, .z = 0},
          Vector3{.x = 0, .y = 1, .z = 0},
          45.0F,
          render::CameraProjection::CAMERA_ORTHOGRAPHIC,
      } {
    _window.setTargetFPS(kTargetFPS);
}

int GUI::run(const GuiCliParser& cli) {
    _address = zappy::network::Address{std::string{cli.host()}, cli.port()};

    _buffer.connect(_address);
    _handshake.run();

    _poller.add(_buffer.fd(), zappy::io::Poller::kPollRead | zappy::io::Poller::kPollError, [this](std::byte events) {
        if ((events & zappy::io::Poller::kPollError) != zappy::io::Poller::kPollNone) {
            throw exception::InvalidState{"server disconnected"};
        }
        _buffer.poll();
        while (_buffer.hasMessages()) {
            _parser.dispatch(_buffer.popMessage());
        }
    });

    while (!_window.shouldClose()) {
        _poller.poll(kPollTimeoutMs);
        _window.beginFrame();
        _window.endFrame();
    }
    return 0;
}

}  // namespace zappy::gui
