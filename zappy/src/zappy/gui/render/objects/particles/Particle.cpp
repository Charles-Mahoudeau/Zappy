/*
** EPITECH PROJECT, 2026
** projectMirror
** File description:
** Particle
*/

#include "Particle.hpp"

#include <raylib.h>

#include <algorithm>

#include "Camera.hpp"
#include "ColorF.hpp"
#include "Texture.hpp"
#include "TimeValue.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"

namespace zappy::gui::render {
void Particle::setPosition(Vector3 position, Vector3 increment) { _position = TimeValue<Vector3>{position, increment}; }

void Particle::setSize(Vector2 size, Vector2 increment) { _size = TimeValue<Vector2>{size, increment}; }

void Particle::setLifetime(float lifetime) { _lifetime = TimeValue<float>{lifetime, 0.0F}; }

void Particle::setRotation(float rotation, float increment) { _rotation = TimeValue<float>{rotation, increment}; }

void Particle::setTint(ColorF tint, ColorF increment) { _tint = TimeValue<ColorF>{tint, increment}; }

void Particle::setIncrementValues(Vector3 positionIncrement, Vector2 sizeIncrement, float rotationIncrement,
                                  ColorF tintIncrement) {
    _position = TimeValue<Vector3>{_position.get(), positionIncrement};
    _size = TimeValue<Vector2>{_size.get(), sizeIncrement};
    _rotation = TimeValue<float>{_rotation.get(), rotationIncrement};
    _tint = TimeValue<ColorF>{_tint.get(), tintIncrement};
}

void Particle::setInitValues(Vector3 position, Vector2 size, float rotation, ColorF tint) {
    _position = TimeValue<Vector3>{position, Vector3{0.0F, 0.0F, 0.0F}};
    _size = TimeValue<Vector2>{size, Vector2{0.0F, 0.0F}};
    _rotation = TimeValue<float>{rotation, 0.0F};
    _tint = TimeValue<ColorF>{tint, ColorF{0.0F, 0.0F, 0.0F, 0.0F}};
}

void Particle::update(float dt) {
    _position.update(dt);
    _position.setIncrement(_position.increment() + (_acceleration * dt));
    _size.update(dt);
    _rotation.update(dt);
    _tint.update(dt);
    _elapsedTime += dt;
}

void Particle::draw(Camera& camera, Texture& texture) {
    const float texSize = static_cast<float>(std::min(texture.width(), texture.height()));
    const Rectangle source{.x = 0.0F, .y = 0.0F, .width = texSize, .height = texSize};
    const Vector2 origin{_size.get().x() * 0.5F, _size.get().y() * 0.5F};
    const Vector3 up{0.0F, 1.0F, 0.0F};
    const Vector2 size{std::max(_size.get().x(), 0.0F), std::max(_size.get().y(), 0.0F)};

    DrawBillboardPro(camera, texture, source, _position.get(), up, size, origin, _rotation.get(),
                     _tint.get().toColor());
}

}  // namespace zappy::gui::render
