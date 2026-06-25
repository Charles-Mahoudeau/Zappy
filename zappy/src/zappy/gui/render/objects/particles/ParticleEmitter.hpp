/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** ParticleEmitter
*/

#pragma once

#include <raylib.h>

#include <string_view>

#include "Color.hpp"
#include "Particle.hpp"
#include "Texture.hpp"
#include "TimeValue.hpp"
#include "Vec2D.hpp"
#include "Vector3.hpp"
#include "vector"

namespace zappy::gui::render {

class ParticleEmitter {
  public:
    explicit ParticleEmitter(std::string_view path);
    ~ParticleEmitter() = default;

    ParticleEmitter(const ParticleEmitter&) = delete;
    ParticleEmitter& operator=(const ParticleEmitter&) = delete;
    ParticleEmitter(ParticleEmitter&&) noexcept = default;
    ParticleEmitter& operator=(ParticleEmitter&&) noexcept = default;

    void setOrigin(Vector3 origin);
    void setVolume(Vector3 volume);
    void setSpread(float spread);
    void setSpeed(float speed);
    void setRate(float rate);
    void setLifetime(float lifetime);
    void setSize(Vec2D size, Vec2D increment);
    void setRotation(float rotation, float increment);
    void setTint(Color tint, Color increment);

    void set(Vector3 origin, Vector3 volume, float spread, float rate, float lifetime);

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
    float _spread{0.0F};
    float _speed{1.0F};
    float _rate{1.0F};
    float _lifetime{1.0F};
    TimeValue<Vec2D> _size{Vec2D{1.0F, 1.0F}, Vec2D{0.0F, 0.0F}};
    TimeValue<float> _rotation{0.0F, 0.0F};
    TimeValue<Color> _tint{WHITE, WHITE};
};

}  // namespace zappy::gui::render
