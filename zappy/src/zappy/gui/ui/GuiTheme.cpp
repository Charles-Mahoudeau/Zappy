/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** GuiTheme
*/

#include "GuiTheme.hpp"

#include <filesystem>
#include <string>
#include <string_view>

#include "raygui.h"

namespace zappy::gui::ui {

GuiTheme::GuiTheme(std::string_view styleFile) {
    if (!std::filesystem::is_regular_file(styleFile)) {
        return;
    }
    GuiLoadStyle(std::string{styleFile}.c_str());
    _loaded = true;
}

GuiTheme::~GuiTheme() {
    if (_loaded) {
        GuiLoadStyleDefault();
    }
}

GuiTheme::GuiTheme(GuiTheme&& other) noexcept : _loaded(other._loaded) { other._loaded = false; }

GuiTheme& GuiTheme::operator=(GuiTheme&& other) noexcept {
    if (this != &other) {
        if (_loaded && !other._loaded) {
            GuiLoadStyleDefault();
        }
        _loaded = other._loaded;
        other._loaded = false;
    }
    return *this;
}

}  // namespace zappy::gui::ui
