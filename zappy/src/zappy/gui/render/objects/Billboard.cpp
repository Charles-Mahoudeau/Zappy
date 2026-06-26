/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Billboard
*/

#include "Billboard.hpp"

#include <raylib.h>

#include <algorithm>
#include <string_view>

#include "../Camera.hpp"
#include "Texture.hpp"
#include "zappy/gui/render/utils/Color.hpp"
#include "zappy/gui/render/utils/Rectangle.hpp"
#include "zappy/gui/render/utils/Vector2.hpp"
#include "zappy/gui/render/utils/Vector3.hpp"

namespace zappy::gui::render {
Billboard::Billboard(std::string_view path) : Texture(path) {
    _source = Rectangle{0.0F, 0.0F, static_cast<float>(width()), static_cast<float>(height())};
}

void Billboard::setSimpleBillboard(Vector3 position, float size, Color tint) {
    _position = position;
    setBillboardSize(size);
    _tint = tint;
    _source = Rectangle{0.0F, 0.0F, static_cast<float>(width()), static_cast<float>(height())};
}

void Billboard::setBillboard(Vector3 position, Vector2 size, float rotation, Color tint, Vector3 up) {
    _position = position;
    _up = up;
    _size = size;
    _rotation = rotation;
    _tint = tint;
    _origin = Vector2{size.x() * 0.5F, size.y() * 0.5F};
}

void Billboard::setPosition(Vector3 position) { _position = position; }

void Billboard::setBillboardSize(float size) { _size = Vector2{size, size}; }

void Billboard::setSource(Rectangle source) { _source = source; }

void Billboard::setUp(Vector3 up) { _up = up; }

void Billboard::setSize(Vector2 size) { _size = size; }

void Billboard::setOrigin(Vector2 origin) { _origin = origin; }

void Billboard::setRotation(float rotation) { _rotation = rotation; }

void Billboard::setTint(Color tint) { _tint = tint; }

Vector3 Billboard::position() const { return _position; }

float Billboard::billboardSize() const { return std::min(_size.x(), _size.y()); }

Rectangle Billboard::source() const { return _source; }

Vector3 Billboard::up() const { return _up; }

Vector2 Billboard::size() const { return _size; }

Vector2 Billboard::origin() const { return _origin; }

float Billboard::rotation() const { return _rotation; }

Color Billboard::tint() const { return _tint; }

void Billboard::draw(Camera& camera) const { DrawBillboard(camera, *this, _position, billboardSize(), _tint); }

void Billboard::drawPro(Camera& camera) const {
    DrawBillboardPro(camera, *this, _source, _position, _up, _size, _origin, _rotation, _tint);
}

}  // namespace zappy::gui::render
