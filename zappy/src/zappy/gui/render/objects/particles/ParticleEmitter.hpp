/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Billboard
*/

#pragma once

#include <raylib.h>

#include "Billboard.hpp"

namespace zappy::gui::render {

class ParticleEmitter : public Texture {
  public:
    explicit ParticleEmitter(std::string_view path);
    ~ParticleEmitter() = default;

    ParticleEmitter(const ParticleEmitter&) = delete;

  private:
    Vector3 _origin{.x = 0.0F, .y = 0.0F, .z = 0.0F};
    Vector3 _volume{.x = 1.0F, .y = 1.0F, .z = 1.0F};
    float _spread{0.0F};
    float _rate{1.0F};
    float _size{1.0F};
    float _lifetime{1.0F};
    float _speed{1.0F};
    float _rotation{0.0F};
    Color _tint{WHITE};
};

class BillboardException : public std::runtime_error {
  public:
    explicit BillboardException(const std::string& message) : std::runtime_error(message) {}
};
}  // namespace zappy::gui::render
