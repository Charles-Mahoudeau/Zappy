/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** GuiTheme
*/

#pragma once

#include <string_view>

namespace zappy::gui::ui {

class GuiTheme {
  public:
    explicit GuiTheme(std::string_view styleFile);
    ~GuiTheme();

    GuiTheme(const GuiTheme&) = delete;
    GuiTheme& operator=(const GuiTheme&) = delete;

    GuiTheme(GuiTheme&& other) noexcept;
    GuiTheme& operator=(GuiTheme&& other) noexcept;

  private:
    bool _loaded{false};
};

}  // namespace zappy::gui::ui
