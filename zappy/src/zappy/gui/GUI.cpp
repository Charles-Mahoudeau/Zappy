/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** GUI
*/

#include "zappy/gui/GUI.hpp"

#include <raylib.h>

#include <algorithm>
#include <cstddef>
#include <format>
#include <string>

#include "zappy/gui/GuiCliParser.hpp"
#include "zappy/gui/display/Window.hpp"
#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/render/Camera.hpp"
#include "zappy/gui/render/Renderer.hpp"
#include "zappy/gui/render/utils/Vector3.hpp"
#include "zappy/shared/exception/InvalidState.hpp"
namespace zappy::gui {

static constexpr int kWindowWidth = 1600;
static constexpr int kWindowHeight = 900;
static constexpr int kTargetFPS = 60;
static constexpr int kPollTimeoutMs = 16;
static constexpr int kLoadingFontSize = 30;
static constexpr int kLoadingMaxDots = 3;

GUI::GUI() : _parser{_state}, _sender{_buffer}, _handshake{_buffer, _sender, _parser, _state} {}

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

void GUI::setupCamera() {
    const auto width = static_cast<float>(_state.width());
    const auto height = static_cast<float>(_state.height());
    const float span = std::max({width, height, 1.0F});
    const float centerX = width / 2.0F;
    const float centerZ = height / 2.0F;
    const float offset = span;
    const float fovy = span * 1.5F;

    _camera = render::Camera{render::Vector3(centerX + offset, span, centerZ + offset),
                             render::Vector3(centerX, 0.0F, centerZ), render::Vector3(0, 1, 0), fovy,
                             render::CameraProjection::CAMERA_ORTHOGRAPHIC};
    _camera.setCameraMode(render::CameraMode::CAMERA_CUSTOM);
}

void GUI::drawLoadingFrame() {
    _loadingDots = (_loadingDots % kLoadingMaxDots) + 1;
    const std::string text = std::format("Loading{}", std::string(static_cast<std::size_t>(_loadingDots), '.'));

    static constexpr const char* kWidestText = "Loading...";
    const int textX = (kWindowWidth - MeasureText(kWidestText, kLoadingFontSize)) / 2;

    _window.beginFrame();
    DrawText(text.c_str(), textX, (kWindowHeight - kLoadingFontSize) / 2, kLoadingFontSize, WHITE);
    _window.endFrame();
}

int GUI::init(const GuiCliParser& cli) {
    // TODO: Not HERE but:
    // InitAudioDevice();
    // if (!IsAudioDeviceReady()) {
    //     throw exception::InvalidState{"Failed to initialize audio device"};
    // }

    connect(cli);

    _window = display::Window{kWindowWidth, kWindowHeight, "Zappy"};
    _window.setTargetFPS(kTargetFPS);
    drawLoadingFrame();

    setupCamera();

    _assets.load([this]() { drawLoadingFrame(); });

    return 0;
}

int GUI::run() {
    _poller.add(_buffer.fd(), zappy::io::Poller::kPollRead | zappy::io::Poller::kPollError, [this](std::byte events) {
        if ((events & zappy::io::Poller::kPollError) != zappy::io::Poller::kPollNone) {
            throw exception::InvalidState{"server disconnected"};
        }
        pump();
    });

    while (!_window.shouldClose()) {
        _poller.poll(kPollTimeoutMs);
        _window.beginFrame();
        _renderer.update(_camera, _state, _assets);
        _window.endFrame();
    }
    return 0;
}

}  // namespace zappy::gui
