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
#include <cstdint>
#include <string_view>

#include "Billboard.hpp"
#include "ColorF.hpp"
#include "Randomvalue.hpp"
#include "TimeValue.hpp"
#include "Vector3.hpp"

namespace zappy::gui::render {
ParticleEmitter::ParticleEmitter(std::string_view path) : _texture(path) {}

void ParticleEmitter::setOrigin(Vector3 origin) { _origin = origin; }
void ParticleEmitter::setVolume(Vector3 volume) { _volume = volume; }
void ParticleEmitter::setSpread(float spread) { _spread = spread; }
void ParticleEmitter::setRate(uint16_t rate) { _rate = rate; }
void ParticleEmitter::setSpeed(float speed, float envelope) { _speed = RandomValue{speed, envelope}; }
void ParticleEmitter::setLifetime(float lifetime, float envelope) { _lifetime = RandomValue{lifetime, envelope}; }
void ParticleEmitter::setSize(Vec2D size, Vec2D increment) { _size = TimeValue<Vec2D>{size, increment}; }
void ParticleEmitter::setRotation(float rotation, float increment) {
    _rotation = TimeValue<float>{rotation, increment};
}
void ParticleEmitter::setTint(ColorF tint, ColorF increment) { _tint = TimeValue<ColorF>{tint, increment}; }

void ParticleEmitter::setStatic(Vector3 origin, Vector3 volume, Vector3 acceleration, float spread, uint16_t rate) {
    _origin = origin;
    _volume = volume;
    _acceleration = acceleration;
    _spread = spread;
    _rate = rate;
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
    Vector3 speed = getDirection() * RandomValue{_speed.value(), _speed.envelope()}.generate();
    float lifetime = RandomValue{_lifetime.value(), _lifetime.envelope()}.generate();
    Vec2D InitSize = _size.get() * RandomValue{1.0F, _InitEnvelope.size}.generate();
    Vec2D incSize = _size.increment() * RandomValue{1.0F, _IncrementEnvelope.size}.generate();
    float initRot = RandomValue{_rotation.get(), _InitEnvelope.rotation}.generate();
    float incRot = RandomValue{_rotation.increment(), _IncrementEnvelope.rotation}.generate();
    ColorF initTint = _tint.get() * RandomValue{1.0F, _InitEnvelope.tint}.generate();
    ColorF incTint = _tint.increment() * RandomValue{1.0F, _IncrementEnvelope.tint}.generate();
    Vector3 origin = _origin + Vector3{RandomValue{0.0F, _volume.x() * 0.5F}.generate(),
                                       RandomValue{0.0F, _volume.y() * 0.5F}.generate(),
                                       RandomValue{0.0F, _volume.z() * 0.5F}.generate()};

    newParticle.setInitValues(origin, InitSize, initRot, initTint);
    newParticle.setIncrementValues(speed, incSize, incRot, incTint);
    newParticle.setLifetime(lifetime);
    newParticle.setAcceleration(_acceleration);
    _particles.push_back(std::move(newParticle));
}

void ParticleEmitter::emit(uint16_t count) {
    for (uint16_t i = 0; i < count; ++i) {
        particle();
    }
}

void ParticleEmitter::emitRate() { emit(_rate); }

Vector3 ParticleEmitter::getDirection() {
    const float maxAngle = (std::clamp(_spread, 0.0f, 90.0f)) * (3.14159265f / 180.0f);

    float inclination = static_cast<float>(GetRandomValue(0, 1000)) / 1000.0f * maxAngle;
    float azimuth = static_cast<float>(GetRandomValue(0, 1000)) / 1000.0f * 2.0f * 3.14159265f;

    float x = std::sin(inclination) * std::cos(azimuth);
    float y = std::cos(inclination);
    float z = std::sin(inclination) * std::sin(azimuth);

    return Vector3{x, y, z};
}

void ParticleEmitter::setInitParticles(Vec2D size, float rotation, ColorF tint, float lifetime, float speed) {
    _size = TimeValue<Vec2D>{size, Vec2D{0.0F, 0.0F}};
    _rotation = TimeValue<float>{rotation, 0.0F};
    _tint = TimeValue<ColorF>{tint, ColorF{0, 0, 0, 0}};
    _lifetime.setValue(lifetime);
    _speed.setValue(speed);
}

void ParticleEmitter::setIncrementParticles(Vec2D sizeIncrement, float rotationIncrement, ColorF tintIncrement) {
    _size = TimeValue<Vec2D>{_size.get(), sizeIncrement};
    _rotation = TimeValue<float>{_rotation.get(), rotationIncrement};
    _tint = TimeValue<ColorF>{_tint.get(), tintIncrement};
}

void ParticleEmitter::setInitEnvelope(ParticleEnvelope envelope, float lifetime, float speed) {
    _InitEnvelope = envelope;
    _lifetime.setEnvelope(lifetime);
    _speed.setEnvelope(speed);
}

void ParticleEmitter::setIncrementEnvelope(ParticleEnvelope envelope) { _IncrementEnvelope = envelope; }

void ParticleEmitter::setStaticParticles(Vec2D size, float rotation, ColorF tint, float lifetime, float speed) {
    _size = TimeValue<Vec2D>{size, Vec2D{0.0F, 0.0F}};
    _rotation = TimeValue<float>{rotation, 0.0F};
    _tint = TimeValue<ColorF>{tint, ColorF{0, 0, 0, 0}};
    _lifetime.setValue(lifetime);
    _speed.setValue(speed);
}
}  // namespace zappy::gui::render
