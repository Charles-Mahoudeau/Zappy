/*
** EPITECH PROJECT, 2026
** projectMirror
** File description:
** Texture
*/

#include "Texture.hpp"

namespace zappy::gui::render {
Texture::Texture(const char* path) { _texture = LoadTexture(path); }

Texture::~Texture() { UnloadTexture(_texture); }

std::uint32_t Texture::id() const { return _texture.id; }

int Texture::width() const { return _texture.width; }

int Texture::height() const { return _texture.height; }
}  // namespace zappy::gui::render
