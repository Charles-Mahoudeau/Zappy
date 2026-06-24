/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Billboard
*/

#pragma once

#include <raylib.h>

#include <string_view>

#include "Billboard.hpp"
#include "Color.hpp"
#include "Particle.hpp"
#include "TimeValue.hpp"
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
    void setRate(float rate);
    void setLifetime(float lifetime);
    void setPosition(Vector3 position, Vector3 increment);
    void setSize(float size, float increment);
    void setSpeed(float speed, float increment);
    void setRotation(float rotation, float increment);
    void setTint(Color tint, Color increment);

    void set(Vector3 origin, Vector3 volume, float spread, float rate, float lifetime);

    void update(float dt);

  private:
    Billboard _billboard;
    std::vector<Particle> _particles;
    Vector3 _origin{0.0F, 0.0F, 0.0F};
    Vector3 _volume{1.0F, 1.0F, 1.0F};
    float _spread{0.0F};
    float _rate{1.0F};
    float _lifetime{1.0F};
    TimeValue<Vector3> _position{Vector3{0.0F, 0.0F, 0.0F}, Vector3{0.0F, 0.0F, 0.0F}};
    TimeValue<float> _size{1.0F, 0.0F};
    TimeValue<float> _speed{1.0F, 0.0F};
    TimeValue<float> _rotation{0.0F, 0.0F};
    TimeValue<Color> _tint{WHITE, WHITE};
};

class BillboardException : public std::runtime_error {
  public:
    explicit BillboardException(const std::string& message) : std::runtime_error(message) {}
};
}  // namespace zappy::gui::render
