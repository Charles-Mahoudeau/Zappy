/*
** EPITECH PROJECT, 2026
** projectMirror
** File description:
** ParticleEmitter
*/

#include "ParticleEmitter.hpp"

#include <raylib.h>

#include <string_view>

#include "Billboard.hpp"
#include "Color.hpp"
#include "TimeValue.hpp"
#include "Vector3.hpp"

namespace zappy::gui::render {
ParticleEmitter::ParticleEmitter(std::string_view path) : _billboard(path) {}

void ParticleEmitter::setOrigin(Vector3 origin) { _origin = origin; }
void ParticleEmitter::setVolume(Vector3 volume) { _volume = volume; }
void ParticleEmitter::setSpread(float spread) { _spread = spread; }
void ParticleEmitter::setRate(float rate) { _rate = rate; }
void ParticleEmitter::setLifetime(float lifetime) { _lifetime = lifetime; }
void ParticleEmitter::setPosition(Vector3 position, Vector3 increment) {
    _position = TimeValue<Vector3>{position, increment};
}
void ParticleEmitter::setSize(float size, float increment) { _size = TimeValue<float>{size, increment}; }
void ParticleEmitter::setSpeed(float speed, float increment) { _speed = TimeValue<float>{speed, increment}; }
void ParticleEmitter::setRotation(float rotation, float increment) {
    _rotation = TimeValue<float>{rotation, increment};
}
void ParticleEmitter::setTint(Color tint, Color increment) { _tint = TimeValue<Color>{tint, increment}; }

void ParticleEmitter::set(Vector3 origin, Vector3 volume, float spread, float rate, float lifetime) {
    _origin = origin;
    _volume = volume;
    _spread = spread;
    _rate = rate;
    _lifetime = lifetime;
}

void ParticleEmitter::update(float dt) {
    for (auto& particle : _particles) {
        particle.update(dt);
    }
}

}  // namespace zappy::gui::render
