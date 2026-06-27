/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** GUI
*/

#include "zappy/gui/GUI.hpp"

#include <raylib.h>

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>

#include "zappy/gui/GuiCliParser.hpp"
#include "zappy/gui/display/Window.hpp"
#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/render/Camera.hpp"
#include "zappy/gui/render/Grid.hpp"
#include "zappy/gui/render/Renderer.hpp"
#include "zappy/gui/render/utils/Color.hpp"
#include "zappy/gui/render/utils/Rectangle.hpp"
#include "zappy/gui/render/utils/Vector3.hpp"
#include "zappy/gui/ui/DotAnimator.hpp"
#include "zappy/gui/ui/Widgets.hpp"
#include "zappy/shared/exception/InvalidState.hpp"
#include "zappy/shared/network/BufferedClient.hpp"

namespace zappy::gui {

static constexpr int kWindowWidth = 1600;
static constexpr int kWindowHeight = 900;
static constexpr int kTargetFPS = 60;
static constexpr int kPollTimeoutMs = 16;
static constexpr int kLoadingFontSize = 30;
static constexpr std::string_view kStylePath = "assets/styles/style_lavanda_kyrou.rgs";
static constexpr int kTimeSliderInitialValue = 1;
static constexpr std::chrono::milliseconds kReconnectDelay{500};

GUI::GUI()
    : _parser{_state},
      _sender{_buffer},
      _handshake{_buffer, _sender, _parser, _state},
      _hud{_sender, kTimeSliderInitialValue} {}

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
    const float width = static_cast<float>(_state.width()) * render::Grid::kSpacing;
    const float height = static_cast<float>(_state.height()) * render::Grid::kSpacing;
    const float span = std::max({width, height, 1.0F});
    const float centerX = width / 2.0F;
    const float centerZ = height / 2.0F;
    const float offset = span;
    const float fovy = span * 1.5F;

    _camera = render::Camera{render::Vector3(centerX + offset, span * 0.85F, centerZ + offset),
                             render::Vector3(centerX, 0.0F, centerZ), render::Vector3(0, 1.0F, 0), fovy,
                             render::CameraProjection::CAMERA_ORTHOGRAPHIC};
    _camera.setCameraMode(render::CameraMode::CAMERA_CUSTOM);
}

void GUI::drawLoadingFrame(std::string_view name, float progress) {
    _dotAnimator.advance();
    const std::string text = _dotAnimator.text("Loading");

    static constexpr int kBarWidth = 400;
    static constexpr int kBarHeight = 24;
    static constexpr int kTextSpacing = 10;
    static constexpr const char* kWidestLoadingText = "Loading...";
    const int barX = (kWindowWidth - kBarWidth) / 2;
    const int barY = (kWindowHeight - kBarHeight) / 2;
    const int loadingTextX = (kWindowWidth - MeasureText(kWidestLoadingText, kLoadingFontSize)) / 2;

    _window.beginFrame();

    const int loadingY = barY - kTextSpacing - kLoadingFontSize;
    DrawText(text.c_str(), loadingTextX, loadingY, kLoadingFontSize, render::Color::kWHITE);

    ui::Widgets::progressBar(render::Rectangle{static_cast<float>(barX), static_cast<float>(barY),
                                               static_cast<float>(kBarWidth), static_cast<float>(kBarHeight)},
                             "", "", progress);

    const std::string nameText{name};
    const int nameY = barY + kBarHeight + kTextSpacing;
    DrawText(nameText.c_str(), barX, nameY, kLoadingFontSize, render::Color::kWHITE);

    _window.endFrame();
}

void GUI::drawWaitingFrame() {
    const std::string text = _dotAnimator.text("Waiting for server");

    const int windowWidth = display::Window::width();
    const int windowHeight = display::Window::height();
    const int textX = (windowWidth - MeasureText("Waiting for server...", kLoadingFontSize)) / 2;
    const int textY = (windowHeight - kLoadingFontSize) / 2;

    _window.beginFrame();
    DrawText(text.c_str(), textX, textY, kLoadingFontSize, render::Color::kWHITE);
    _window.endFrame();
}

bool GUI::tryConnect() {
    try {
        _buffer = zappy::network::BufferedClient{};
        _state = game::GameState{};
        _renderer = render::Renderer{};
        _buffer.connect(_address);
        _handshake.run();
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

bool GUI::waitForConnection() {
    auto nextAttempt = std::chrono::steady_clock::now();

    while (!_window.shouldClose()) {
        if (std::chrono::steady_clock::now() >= nextAttempt) {
            if (tryConnect()) {
                return true;
            }
            nextAttempt = std::chrono::steady_clock::now() + kReconnectDelay;
        }

        _dotAnimator.update(GetFrameTime());
        drawWaitingFrame();
    }
    return false;
}

void GUI::runSession() {
    setupCamera();

    _poller.clear();
    _poller.add(_buffer.fd(), zappy::io::Poller::kPollRead | zappy::io::Poller::kPollError, [this](std::byte events) {
        if ((events & zappy::io::Poller::kPollError) != zappy::io::Poller::kPollNone) {
            throw exception::InvalidState{"server disconnected"};
        }
        pump();
    });

    try {
        while (!_window.shouldClose()) {
            _poller.poll(kPollTimeoutMs);
            _window.beginFrame();
            _hud.update(_camera, _state);
            _camera.followPlayer(_hud.focusedPlayerWorldPosition(_state), !_hud.isMouseOverChatPanel());

            _renderer.update(_camera, _state, _assets);
            _hud.draw(_state);
            _window.endFrame();
        }
    } catch (const std::exception& err) {
        std::cerr << "Server connection lost: " << err.what() << '\n';
    }
}

int GUI::init(const GuiCliParser& cli) {
    // TODO: Not HERE but:
    // InitAudioDevice();
    // if (!IsAudioDeviceReady()) {
    //     throw exception::InvalidState{"Failed to initialize audio device"};
    // }

    _address = zappy::network::Address{std::string{cli.host()}, cli.port()};

    _window = display::Window{kWindowWidth, kWindowHeight, "Zappy"};
    _window.setTargetFPS(kTargetFPS);
    _theme = ui::GuiTheme{kStylePath};
    drawLoadingFrame("", 0.0F);

    _assets.load([this](std::string_view name, float progress) { drawLoadingFrame(name, progress); });

    return 0;
}

int GUI::run() {
    while (!_window.shouldClose()) {
        if (!waitForConnection()) {
            break;
        }
        runSession();
    }
    return 0;
}

}  // namespace zappy::gui
