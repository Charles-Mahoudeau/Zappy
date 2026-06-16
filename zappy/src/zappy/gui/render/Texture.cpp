/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Texture
*/

#include "Texture.hpp"

#include <raylib.h>

#include <cstdint>

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

bool Texture::isValid() const { return _texture.id != 0; }

void Texture::reload(const char* path) {
    UnloadTexture(_texture);
    _texture = LoadTexture(path);
}

void Texture::swap(Texture& other) noexcept {
    const Texture2D tmp = _texture;

    _texture = other._texture;
    other._texture = tmp;
}
}  // namespace zappy::gui::render
