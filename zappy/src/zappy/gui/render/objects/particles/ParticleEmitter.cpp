/*
** EPITECH PROJECT, 2026
** projectMirror
** File description:
** ParticleEmitter
*/

#include "ParticleEmitter.hpp"

#include <raylib.h>

#include <algorithm>
#include <cmath>
#include <string_view>

#include "Billboard.hpp"
#include "Color.hpp"
#include "TimeValue.hpp"
#include "Vector3.hpp"

namespace zappy::gui::render {
ParticleEmitter::ParticleEmitter(std::string_view path) : _texture(path) {}

void ParticleEmitter::setOrigin(Vector3 origin) { _origin = origin; }
void ParticleEmitter::setVolume(Vector3 volume) { _volume = volume; }
void ParticleEmitter::setSpread(float spread) { _spread = spread; }
void ParticleEmitter::setRate(float rate) { _rate = rate; }
void ParticleEmitter::setSpeed(float speed) { _speed = speed; }
void ParticleEmitter::setLifetime(float lifetime) { _lifetime = lifetime; }
void ParticleEmitter::setSize(Vec2D size, Vec2D increment) { _size = TimeValue<Vec2D>{size, increment}; }
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
    for (size_t i = 0; i < _particles.size(); ++i) {
        _particles[i].update(dt);
        if (!_particles[i].isAlive()) {
            _particles.erase(_particles.begin() + i);
            --i;
        }
    }
}

uint16_t ParticleEmitter::draw(Camera& camera) {
    for (auto& particle : _particles) {
        if (particle.lifetime() > 0.0F) {
            particle.draw(camera, _texture);
        }
    }
    return _particles.size();
}

void ParticleEmitter::particle() {
    Particle newParticle;
    Vector3 speed = getDirection() * _speed;
    newParticle.setInitValues(_origin, _size, _rotation.get(), _tint.get());
    newParticle.setIncrementValues(speed, _size, _rotation.increment(), _tint.increment());
    newParticle.setLifetime(_lifetime);
    _particles.push_back(std::move(newParticle));
}

void ParticleEmitter::emit(uint16_t count) {
    for (float i = 0.0F; i < count; i += 1.0F) {
        particle();
    }
}

void ParticleEmitter::emitRate() { emit(static_cast<uint16_t>(_rate)); }

Vector3 ParticleEmitter::getDirection() {
    const float maxAngle = (std::clamp(_spread, 0.0f, 90.0f)) * (3.14159265f / 180.0f);

    float inclination = static_cast<float>(GetRandomValue(0, 1000)) / 1000.0f * maxAngle;
    float azimuth = static_cast<float>(GetRandomValue(0, 1000)) / 1000.0f * 2.0f * 3.14159265f;

    float x = std::sin(inclination) * std::cos(azimuth);
    float y = std::cos(inclination);
    float z = std::sin(inclination) * std::sin(azimuth);

    return Vector3{x, y, z};
}

void ParticleEmitter::setInitParticles(Vec2D size, float rotation, Color tint) {
    _size = TimeValue<Vec2D>{size, Vec2D{0.0F, 0.0F}};
    _rotation = TimeValue<float>{rotation, 0.0F};
    _tint = TimeValue<Color>{tint, Color{0, 0, 0, 0}};
}

void ParticleEmitter::setIncrementParticles(Vec2D sizeIncrement, float rotationIncrement, Color tintIncrement) {
    _size = TimeValue<Vec2D>{_size.get(), sizeIncrement};
    _rotation = TimeValue<float>{_rotation.get(), rotationIncrement};
    _tint = TimeValue<Color>{_tint.get(), tintIncrement};

}  // namespace zappy::gui::render
