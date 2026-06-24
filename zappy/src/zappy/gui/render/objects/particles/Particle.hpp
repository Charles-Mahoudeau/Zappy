/*
** EPITECH PROJECT, 2026
** projectMirror
** File description:
** Particle
*/

#pragma once

#include <raylib.h>

#include <string_view>

#include "Billboard.hpp"
#include "Color.hpp"
#include "TimeValue.hpp"
#include "Vector3.hpp"

namespace zappy::gui::render {
class Particle : public Billboard {
  public:
    Particle(std::string_view path);
    Particle(const Particle&) = delete;
    Particle(Particle&&) noexcept = default;
    ~Particle();

    Particle& operator=(const Particle&) = delete;
    [[nodiscard]] Vector3 position() const { return _position; }
    [[nodiscard]] float size() const { return _size; }
    [[nodiscard]] float lifetime() const { return _lifetime; }
    [[nodiscard]] float speed() const { return _speed; }
    [[nodiscard]] float rotation() const { return _rotation; }
    [[nodiscard]] Color tint() const { return _tint; }

    void setInitValues(Vector3 position, float size, float speed, float rotation, Color tint);

    void setIncrementValues(Vector3 positionIncrement, float sizeIncrement, float speedIncrement,
                            float rotationIncrement, Color tintIncrement);

    void setPosition(Vector3 position, Vector3 increment);
    void setSize(float size, float increment);
    void setLifetime(float lifetime);
    void setSpeed(float speed, float increment);
    void setRotation(float rotation, float increment);
    void setTint(Color tint, Color increment);

    void update(float dt) {
        _position.update(dt);
        _size.update(dt);
        _speed.update(dt);
        _rotation.update(dt);
        _tint.update(dt);
    }

  private:
    float _lifetime{1.0F};
    TimeValue<Vector3> _position{Vector3{0.0F, 0.0F, 0.0F}, Vector3{0.0F, 0.0F, 0.0F}};
    TimeValue<float> _size{1.0F, 0.0F};
    TimeValue<float> _speed{1.0F, 0.0F};
    TimeValue<float> _rotation{0.0F, 0.0F};
    TimeValue<Color> _tint{WHITE, WHITE};
};
}  // namespace zappy::gui::render
