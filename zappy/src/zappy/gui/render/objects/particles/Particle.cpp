/*
** EPITECH PROJECT, 2026
** projectMirror
** File description:
** Particle
*/

#include "Particle.hpp"

#include <raylib.h>

#include <string_view>

#include "Billboard.hpp"
#include "Color.hpp"
#include "TimeValue.hpp"
#include "Vector3.hpp"

namespace zappy::gui::render {
Particle::Particle(std::string_view path) : Billboard(path) {}

Particle::~Particle() = default;

void Particle::setPosition(Vector3 position, Vector3 increment) { _position = TimeValue<Vector3>{position, increment}; }

void Particle::setSize(float size, float increment) { _size = TimeValue<float>{size, increment}; }

void Particle::setLifetime(float lifetime) { _lifetime = TimeValue<float>{lifetime, 0.0F}; }

void Particle::setSpeed(float speed, float increment) { _speed = TimeValue<float>{speed, increment}; }

void Particle::setRotation(float rotation, float increment) { _rotation = TimeValue<float>{rotation, increment}; }

void Particle::setTint(Color tint, Color increment) { _tint = TimeValue<Color>{tint, increment}; }

void Particle::setIncrementValues(Vector3 positionIncrement, float sizeIncrement, float speedIncrement,
                                  float rotationIncrement, Color tintIncrement) {
    _position = TimeValue<Vector3>{_position.get(), positionIncrement};
    _size = TimeValue<float>{_size.get(), sizeIncrement};
    _speed = TimeValue<float>{_speed.get(), speedIncrement};
    _rotation = TimeValue<float>{_rotation.get(), rotationIncrement};
    _tint = TimeValue<Color>{_tint.get(), tintIncrement};
}

void Particle::setInitValues(Vector3 position, float size, float speed, float rotation, Color tint) {
    _position = TimeValue<Vector3>{position, Vector3{0.0F, 0.0F, 0.0F}};
    _size = TimeValue<float>{size, 0.0F};
    _speed = TimeValue<float>{speed, 0.0F};
    _rotation = TimeValue<float>{rotation, 0.0F};
    _tint = TimeValue<Color>{tint, Color{0, 0, 0, 0}};
}

}  // namespace zappy::gui::render
