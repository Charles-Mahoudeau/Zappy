/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** IWindow
*/

#pragma once

namespace zappy::gui::display {
class IWindow {
  public:
    IWindow() = default;
    IWindow(const IWindow&) = default;
    IWindow(IWindow&&) = default;
    virtual ~IWindow() = default;

    IWindow& operator=(const IWindow&) = default;
    IWindow& operator=(IWindow&&) = default;

    [[nodiscard]] virtual bool shouldClose() const = 0;

    virtual void setTargetFPS(int fps) = 0;
    virtual void beginFrame() = 0;
    virtual void endFrame() = 0;
};
}  // namespace zappy::gui::display
