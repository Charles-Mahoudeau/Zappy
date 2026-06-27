/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** ParticleEmitter
*/

#pragma once

#include <cstdint>
#include <string_view>

#include "Color.hpp"
#include "Particle.hpp"
#include "RandomValue.hpp"
#include "Texture.hpp"
#include "TimeValue.hpp"
#include "Vec2D.hpp"
#include "Vector3.hpp"
#include "vector"

namespace zappy::gui::render {

class ParticleEmitter {
  public:
    struct ParticleEnvelope {
        float size{0.0F};
        float rotation{0.0F};
        float tint{0.0F};
    };

    explicit ParticleEmitter(std::string_view path);
    ~ParticleEmitter() = default;

    ParticleEmitter(const ParticleEmitter&) = delete;
    ParticleEmitter& operator=(const ParticleEmitter&) = delete;
    ParticleEmitter(ParticleEmitter&&) noexcept = default;
    ParticleEmitter& operator=(ParticleEmitter&&) noexcept = default;

    void setOrigin(Vector3 origin);
    void setVolume(Vector3 volume);
    void setSpread(float spread);
    void setSpeed(float speed, float envelope = 0.0F);
    void setRate(uint16_t rate);
    void setLifetime(float lifetime, float envelope = 0.0F);
    void setSize(Vec2D size, Vec2D increment);
    void setRotation(float rotation, float increment);
    void setTint(ColorF tint, ColorF increment);

    void setStatic(Vector3 origin, Vector3 volume, Vector3 acceleration, float spread, uint16_t rate);
    void setInitParticles(Vec2D size, float rotation, ColorF tint, float lifetime, float speed);
    void setIncrementParticles(Vec2D sizeIncrement, float rotationIncrement, ColorF tintIncrement);
    void setInitEnvelope(ParticleEnvelope envelope, float lifetime, float speed);
    void setIncrementEnvelope(ParticleEnvelope envelope);

    void setStaticParticles(Vec2D size, float rotation, ColorF tint, float lifetime, float speed);

    [[nodiscard]] Vector3 origin() const { return _origin; }
    [[nodiscard]] Vector3 volume() const { return _volume; }
    [[nodiscard]] float spread() const { return _spread; }
    [[nodiscard]] float speed() const { return _speed.value(); }
    [[nodiscard]] uint16_t rate() const { return _rate; }
    [[nodiscard]] float lifetime() const { return _lifetime.generate(); }
    [[nodiscard]] Vec2D size() const { return _size.get(); }
    [[nodiscard]] float rotation() const { return _rotation.get(); }
    [[nodiscard]] ColorF tint() const { return _tint.get(); }

    [[nodiscard]] std::size_t particleCount() const { return _particles.size(); }

    void update(float dt);
    uint16_t draw(Camera& camera);

    void particle();
    void emit(uint16_t count);
    void emitRate();

  private:
    Vector3 getDirection();
    Texture _texture;
    std::vector<Particle> _particles;
    Vector3 _origin{0.0F, 0.0F, 0.0F};
    Vector3 _volume{1.0F, 1.0F, 1.0F};
    Vector3 _acceleration{0.0F, 0.0F, 0.0F};
    float _spread{0.0F};
    RandomValue _speed{0.0F, 0.0F};
    RandomValue _lifetime{1.0F, 0.0F};
    uint16_t _rate{1};
    TimeValue<Vec2D> _size{Vec2D{1.0F, 1.0F}, Vec2D{0.0F, 0.0F}};
    TimeValue<float> _rotation{0.0F, 0.0F};
    TimeValue<ColorF> _tint{ColorF(Color::kWHITE), ColorF(Color::kWHITE)};
    ParticleEnvelope _InitEnvelope;
    ParticleEnvelope _IncrementEnvelope;
};

}  // namespace zappy::gui::render
