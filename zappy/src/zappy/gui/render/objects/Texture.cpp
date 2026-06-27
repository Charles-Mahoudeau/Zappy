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
Texture::Texture(std::string_view path, bool flipVertical, bool premultiplyAlpha) {
    const std::string pathStr{path};
    Image img = LoadImage(pathStr.c_str());
    if (flipVertical) ImageFlipVertical(&img);
    if (premultiplyAlpha) ImageAlphaPremultiply(&img);
    _texture = LoadTextureFromImage(img);
    UnloadImage(img);
    if (!isValid()) {
        throw TextureException{"Failed to load texture from path: " + pathStr};
    }
}

Texture::Texture(Texture2D texture) : _texture{texture} {
    if (!isValid()) {
        throw TextureException{"Failed to load texture from Texture2D"};
    }
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

void Texture::reload(std::string_view path, bool flipVertical) {
    const std::string pathStr{path};

    UnloadTexture(_texture);
    _texture = loadTexture(pathStr.c_str(), flipVertical);
    if (!isValid()) {
        throw TextureException{"Failed to reload texture from path: " + pathStr};
    }
}

Texture2D Texture::release() noexcept {
    const Texture2D owned = _texture;
    _texture = {};
    return owned;
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
