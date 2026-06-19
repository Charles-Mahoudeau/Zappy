/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Texture
*/

#include "Texture.hpp"

#include <raylib.h>

#include <cstdint>
#include <string>
#include <string_view>

namespace zappy::gui::render {
Texture::Texture(std::string_view path) {
    if (!isValid()) {
        throw TextureException{"Failed to load texture from path: " + std::string{path}};
    }
    _texture = LoadTexture(path.data());
}

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

bool Texture::isValid() const { return IsTextureValid(_texture); }

void Texture::reload(std::string_view path) {
    UnloadTexture(_texture);
    _texture = LoadTexture(path.data());
}

void Texture::swap(Texture& other) noexcept {
    const Texture2D tmp = _texture;

    _texture = other._texture;
    other._texture = tmp;
}
}  // namespace zappy::gui::render
