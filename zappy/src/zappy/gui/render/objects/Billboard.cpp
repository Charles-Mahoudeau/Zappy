/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Billboard
*/

#include "Billboard.hpp"

#include <raylib.h>

#include <cstdint>
#include <string>

namespace zappy::gui::render {
Billboard::Billboard(std::string_view path) : Texture(path) {}

Billboard::~Billboard() = default;


}  // namespace zappy::gui::render
