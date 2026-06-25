/*
** EPITECH PROJECT, 2026
** projectMirror
** File description:
** Particle
*/

#include "Particle.hpp"

#include <raylib.h>

#include "../../Camera.hpp"
#include "../../utils/Color.hpp"
#include "../../utils/TimeValue.hpp"
#include "../../utils/Vec2D.hpp"
#include "../../utils/Vector3.hpp"
#include "../Texture.hpp"

namespace zappy::gui::render {
Particle::Particle() = default;

Particle::~Particle() = default;

void Particle::setPosition(Vector3 position, Vector3 increment) { _position = TimeValue<Vector3>{position, increment}; }

void Particle::setSize(Vec2D size, Vec2D increment) { _size = TimeValue<Vec2D>{size, increment}; }

void Particle::setLifetime(float lifetime) { _lifetime = TimeValue<float>{lifetime, 0.0F}; }

void Particle::setRotation(float rotation, float increment) { _rotation = TimeValue<float>{rotation, increment}; }

void Particle::setTint(Color tint, Color increment) { _tint = TimeValue<Color>{tint, increment}; }

void Particle::setIncrementValues(Vector3 positionIncrement, Vec2D sizeIncrement, float rotationIncrement,
                                  Color tintIncrement) {
    _position = TimeValue<Vector3>{_position.get(), positionIncrement};
    _size = TimeValue<Vec2D>{_size.get(), sizeIncrement};
    _rotation = TimeValue<float>{_rotation.get(), rotationIncrement};
    _tint = TimeValue<Color>{_tint.get(), tintIncrement};
}

void Particle::setInitValues(Vector3 position, Vec2D size, float rotation, Color tint) {
    _position = TimeValue<Vector3>{position, Vector3{0.0F, 0.0F, 0.0F}};
    _size = TimeValue<Vec2D>{size, Vec2D{0.0F, 0.0F}};
    _rotation = TimeValue<float>{rotation, 0.0F};
    _tint = TimeValue<Color>{tint, Color{0, 0, 0, 0}};
}

void Particle::update(float dt) {
    _position.update(dt);
    _size.update(dt);
    _rotation.update(dt);
    _tint.update(dt);
}

void Particle::draw(Camera& camera, Texture& texture) {
    const Rectangle source{.x = 0.0F,
                           .y = 0.0F,
                           .width = static_cast<float>(texture.width()),
                           .height = static_cast<float>(texture.height())};
    const Vector3 up{0.0F, 1.0F, 0.0F};
    const Vector2 _origin{.x = 0.5F, .y = 0.5F};

    DrawBillboardPro(camera, texture, source, _position.get(), up, _size.get(), _origin, _rotation.get(), _tint.get());
}

}  // namespace zappy::gui::render
