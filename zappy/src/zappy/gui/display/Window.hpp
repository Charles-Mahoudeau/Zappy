/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Window
*/

#pragma once

#include <string_view>

#include "zappy/gui/render/Camera.hpp"

namespace zappy::gui::display {
class Window {
  public:
    Window() = default;
    Window(int width, int height, std::string_view title);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    Window(Window&& other) noexcept;
    Window& operator=(Window&& other) noexcept;

    [[nodiscard]] bool shouldClose() const;

    void setTargetFPS(int fps);
    void beginFrame();
    void endFrame();

    static void BeginMode3D(render::Camera& camera3D);
    static void EndMode3D();

  private:
    bool _open{false};
};
}  // namespace zappy::gui::display
