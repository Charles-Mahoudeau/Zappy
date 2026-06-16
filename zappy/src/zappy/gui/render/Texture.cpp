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

Texture::Texture(Texture&& other) noexcept : _texture(other._texture) { other._texture = {}; }

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        UnloadTexture(_texture);
        _texture = other._texture;
        other._texture = {};
    }
    return *this;
}

std::uint32_t Texture::id() const { return _texture.id; }

int Texture::width() const { return _texture.width; }

int Texture::height() const { return _texture.height; }
}  // namespace zappy::gui::render
