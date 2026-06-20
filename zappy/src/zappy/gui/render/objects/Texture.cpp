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

Texture::Texture(const char* path, bool flipVertical) { _texture = loadTexture(path, flipVertical); }

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

void Texture::reload(const char* path) {
    UnloadTexture(_texture);
    _texture = LoadTexture(path);
}

void Texture::swap(Texture& other) noexcept {
    const Texture2D tmp = _texture;

    _texture = other._texture;
    other._texture = tmp;
}

Texture2D Texture::loadTexture(const char* path, bool flipVertical) {
    if (!flipVertical) {
        return LoadTexture(path);
    }
    Image image = LoadImage(path);
    ImageFlipVertical(&image);
    const Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);
    return texture;
}
}  // namespace zappy::gui::render
