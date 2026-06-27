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
#include <cstddef>
#include <cstdint>
#include <numbers>
#include <string_view>
#include <utility>

#include "Camera.hpp"
#include "ColorF.hpp"
#include "RandomValue.hpp"
#include "Texture.hpp"
#include "TimeValue.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"

namespace zappy::gui::render {
ParticleEmitter::ParticleEmitter(std::string_view path) : _texture(path) {}

void ParticleEmitter::setOrigin(Vector3 origin) { _origin = origin; }
void ParticleEmitter::setVolume(Vector3 volume) { _volume = volume; }
void ParticleEmitter::setSpread(float spread) { _spread = spread; }
void ParticleEmitter::setRate(uint16_t rate) { _rate = rate; }
void ParticleEmitter::setSpeed(float speed, float envelope) { _speed = RandomValue{speed, envelope}; }
void ParticleEmitter::setLifetime(float lifetime, float envelope) { _lifetime = RandomValue{lifetime, envelope}; }
void ParticleEmitter::setSize(Vector2 size, Vector2 increment) { _size = TimeValue<Vector2>{size, increment}; }
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
    for (std::size_t i = 0; i < _particles.size(); ++i) {
        _particles.at(i).update(dt);
        if (!_particles.at(i).isAlive()) {
            _particles.erase(_particles.begin() + static_cast<std::ptrdiff_t>(i));
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
    return static_cast<uint16_t>(_particles.size());
}

void ParticleEmitter::particle() {
    Particle newParticle;
    const Vector3 speed = getDirection() * RandomValue{_speed.value(), _speed.envelope()}.generate();
    const float lifetime = RandomValue{_lifetime.value(), _lifetime.envelope()}.generate();
    const Vector2 initSize = _size.get() * RandomValue{1.0F, _initEnvelope.size}.generate();
    const Vector2 incSize = _size.increment() * RandomValue{1.0F, _incrementEnvelope.size}.generate();
    const float initRot = RandomValue{_rotation.get(), _initEnvelope.rotation}.generate();
    const float incRot = RandomValue{_rotation.increment(), _incrementEnvelope.rotation}.generate();
    const ColorF initTint = _tint.get() * RandomValue{1.0F, _initEnvelope.tint}.generate();
    const ColorF incTint = _tint.increment() * RandomValue{1.0F, _incrementEnvelope.tint}.generate();
    const Vector3 origin = _origin + Vector3{RandomValue{0.0F, _volume.x() * 0.5F}.generate(),
                                             RandomValue{0.0F, _volume.y() * 0.5F}.generate(),
                                             RandomValue{0.0F, _volume.z() * 0.5F}.generate()};

    newParticle.setInitValues(origin, initSize, initRot, initTint);
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

Vector3 ParticleEmitter::getDirection() const {
    const float maxAngle = std::clamp(_spread, 0.0F, 90.0F) * (std::numbers::pi_v<float> / 180.0F);

    const float inclination = (static_cast<float>(GetRandomValue(0, 1000)) / 1000.0F) * maxAngle;
    const float azimuth = (static_cast<float>(GetRandomValue(0, 1000)) / 1000.0F) * 2.0F * std::numbers::pi_v<float>;

    const float x = std::sin(inclination) * std::cos(azimuth);
    const float y = std::cos(inclination);
    const float z = std::sin(inclination) * std::sin(azimuth);

    return Vector3{x, y, z};
}

void ParticleEmitter::setInitParticles(Vector2 size, float rotation, ColorF tint, float lifetime, float speed) {
    _size = TimeValue<Vector2>{size, Vector2{0.0F, 0.0F}};
    _rotation = TimeValue<float>{rotation, 0.0F};
    _tint = TimeValue<ColorF>{tint, ColorF{0.0F, 0.0F, 0.0F, 0.0F}};
    _lifetime.setValue(lifetime);
    _speed.setValue(speed);
}

void ParticleEmitter::setIncrementParticles(Vector2 sizeIncrement, float rotationIncrement, ColorF tintIncrement) {
    _size = TimeValue<Vector2>{_size.get(), sizeIncrement};
    _rotation = TimeValue<float>{_rotation.get(), rotationIncrement};
    _tint = TimeValue<ColorF>{_tint.get(), tintIncrement};
}

void ParticleEmitter::setInitEnvelope(ParticleEnvelope envelope, float lifetime, float speed) {
    _initEnvelope = envelope;
    _lifetime.setEnvelope(lifetime);
    _speed.setEnvelope(speed);
}

void ParticleEmitter::setIncrementEnvelope(ParticleEnvelope envelope) { _incrementEnvelope = envelope; }

void ParticleEmitter::setStaticParticles(Vector2 size, float rotation, ColorF tint, float lifetime, float speed) {
    _size = TimeValue<Vector2>{size, Vector2{0.0F, 0.0F}};
    _rotation = TimeValue<float>{rotation, 0.0F};
    _tint = TimeValue<ColorF>{tint, ColorF{0.0F, 0.0F, 0.0F, 0.0F}};
    _lifetime.setValue(lifetime);
    _speed.setValue(speed);
}
}  // namespace zappy::gui::render
