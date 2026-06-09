/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Window
*/

#pragma once

#include <string_view>

#include "IWindow.hpp"

namespace zappy::gui::display {
class Window : public IWindow {
  public:
    Window(int width, int height, std::string_view title);
    ~Window() override;

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    Window(Window&& other) noexcept;
    Window& operator=(Window&& other) noexcept;

    [[nodiscard]] bool shouldClose() const override;

    void setTargetFPS(int fps) override;
    void beginFrame() override;
    void endFrame() override;

  private:
    bool _open{false};
};
}  // namespace zappy::gui::display
