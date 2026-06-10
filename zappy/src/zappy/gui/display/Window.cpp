/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Window
*/

#include "Window.hpp"

#include <raylib.h>

#include <string>
#include <string_view>

#include "zappy/shared/exception/WindowException.hpp"

namespace zappy::gui::display {

Window::Window(int width, int height, std::string_view title) {
    InitWindow(width, height, std::string{title}.c_str());
    if (!IsWindowReady()) {
        throw exception::WindowException{"Failed to open window"};
    }
    _open = true;
}

Window::~Window() {
    if (_open) {
        CloseWindow();
    }
}

Window::Window(Window&& other) noexcept : _open{other._open} { other._open = false; }

Window& Window::operator=(Window&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    if (_open) {
        CloseWindow();
    }
    _open = other._open;
    other._open = false;
    return *this;
}

// NOLINTNEXTLINE
bool Window::shouldClose() const { return WindowShouldClose(); }

// NOLINTNEXTLINE
void Window::setTargetFPS(int fps) { SetTargetFPS(fps); }

// NOLINTNEXTLINE
void Window::beginFrame() {
    BeginDrawing();
    ClearBackground(BLACK);
}

// NOLINTNEXTLINE
void Window::endFrame() { EndDrawing(); }

}  // namespace zappy::gui::display
