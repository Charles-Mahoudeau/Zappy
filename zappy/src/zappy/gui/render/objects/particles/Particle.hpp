/*
** EPITECH PROJECT, 2026
** projectMirror
** File description:
** Particle
*/

#pragma once

#include <raylib.h>

#include "Camera.hpp"
#include "Color.hpp"
#include "ColorF.hpp"
#include "Texture.hpp"
#include "TimeValue.hpp"
#include "Vec2D.hpp"
#include "Vector3.hpp"

namespace zappy::gui::render {
class Particle {
  public:
    Particle() = default;
    Particle(const Particle&) = default;
    Particle(Particle&&) noexcept = default;
    ~Particle() = default;

    Particle& operator=(const Particle&) = default;
    [[nodiscard]] Vector3 position() const { return _position; }
    [[nodiscard]] Vec2D size() const { return _size; }
    [[nodiscard]] float lifetime() const { return _lifetime; }
    [[nodiscard]] float rotation() const { return _rotation; }
    [[nodiscard]] ColorF tint() const { return _tint; }
    [[nodiscard]] float elapsedTime() const { return _elapsedTime; }
    [[nodiscard]] bool isAlive() const { return _elapsedTime < _lifetime; }

    void setInitValues(Vector3 position, Vec2D size, float rotation, ColorF tint);

    void setIncrementValues(Vector3 positionIncrement, Vec2D sizeIncrement, float rotationIncrement,
                            ColorF tintIncrement);

    void setPosition(Vector3 position, Vector3 increment);
    void setSize(Vec2D size, Vec2D increment);
    void setLifetime(float lifetime);
    void setRotation(float rotation, float increment);
    void setTint(ColorF tint, ColorF increment);

    void update(float dt);

    void draw(Camera& camera, Texture& texture);

  private:
    float _lifetime{1.0F};
    float _elapsedTime{0.0F};
    TimeValue<Vector3> _position{Vector3{0.0F, 0.0F, 0.0F}, Vector3{0.0F, 0.0F, 0.0F}};
    TimeValue<Vec2D> _size{Vec2D{1.0F, 1.0F}, Vec2D{0.0F, 0.0F}};
    TimeValue<float> _rotation{0.0F, 0.0F};
    TimeValue<ColorF> _tint{Color::kWHITE, Color::kWHITE};
};
}  // namespace zappy::gui::render
