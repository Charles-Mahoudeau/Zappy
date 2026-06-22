/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Billboard
*/

#include "Billboard.hpp"

#include <raylib.h>

#include <string_view>

#include "../Camera.hpp"
#include "Texture.hpp"

namespace zappy::gui::render {
Billboard::Billboard(std::string_view path) : Texture(path) {}

Billboard::Billboard(std::string_view path, Vector3 position, float size, Color tint)
    : Texture(path), _position(position), _billboardSize(size), _tint(tint) {
    _source =
        Rectangle{.x = 0.0F, .y = 0.0F, .width = static_cast<float>(width()), .height = static_cast<float>(height())};
}

Billboard::Billboard(std::string_view path, Rectangle source, Vector3 position, Vector3 up, Vector2 size,
                     Vector2 origin, float rotation, Color tint)
    : Texture(path),
      _source(source),
      _position(position),
      _up(up),
      _size(size),
      _origin(origin),
      _rotation(rotation),
      _tint(tint),
      _usePro(true) {}

void Billboard::setPosition(Vector3 position) { _position = position; }

void Billboard::setBillboardSize(float size) {
    _billboardSize = size;
    _usePro = false;
}

void Billboard::setSource(Rectangle source) { _source = source; }

void Billboard::setUp(Vector3 up) { _up = up; }

void Billboard::setProSize(Vector2 size) {
    _size = size;
    _usePro = true;
}

void Billboard::setOrigin(Vector2 origin) { _origin = origin; }

void Billboard::setRotation(float rotation) { _rotation = rotation; }

void Billboard::setTint(Color tint) { _tint = tint; }

Vector3 Billboard::position() const { return _position; }

float Billboard::billboardSize() const { return _billboardSize; }

Rectangle Billboard::source() const { return _source; }

Vector3 Billboard::up() const { return _up; }

Vector2 Billboard::proSize() const { return _size; }

Vector2 Billboard::origin() const { return _origin; }

float Billboard::rotation() const { return _rotation; }

Color Billboard::tint() const { return _tint; }

void Billboard::draw(const Camera& camera) const {
    const Camera3D& rlCamera{static_cast<const Camera3D&>(camera)};
    const Texture2D& billboardTexture{static_cast<const Texture2D&>(*this)};

    DrawBillboard(rlCamera, billboardTexture, _position, _billboardSize, _tint);
}

void Billboard::drawPro(const Camera& camera) const {
    const Camera3D& rlCamera{static_cast<const Camera3D&>(camera)};
    const Texture2D& billboardTexture{static_cast<const Texture2D&>(*this)};

    const Rectangle source = _source.width == 0.0F || _source.height == 0.0F
                                 ? Rectangle{.x = 0.0F,
                                             .y = 0.0F,
                                             .width = static_cast<float>(width()),
                                             .height = static_cast<float>(height())}
                                 : _source;
    const Vector2 size = _usePro ? _size : Vector2{.x = source.width / source.height, .y = 1.0F};
    const Vector2 origin = _usePro ? _origin : Vector2{.x = size.x * 0.5F, .y = size.y * 0.5F};

    DrawBillboardPro(rlCamera, billboardTexture, source, _position, _up, size, origin, _rotation, _tint);
}

}  // namespace zappy::gui::render
