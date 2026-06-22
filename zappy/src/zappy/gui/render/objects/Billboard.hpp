/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Billboard
*/

#pragma once

#include <raylib.h>

#include <stdexcept>
#include <string>
#include <string_view>

#include "Texture.hpp"

namespace zappy::gui::render {
class Camera;
}

namespace zappy::gui::render {

class Billboard : public Texture {
  public:
    explicit Billboard(std::string_view path);
    Billboard(std::string_view path, Vector3 position, float size, Color tint = WHITE);
    Billboard(std::string_view path, Rectangle source, Vector3 position, Vector3 up, Vector2 size, Vector2 origin,
              float rotation, Color tint = WHITE);
    ~Billboard() = default;

    Billboard(const Billboard&) = delete;
    Billboard& operator=(const Billboard&) = delete;
    Billboard(Billboard&&) noexcept = default;
    Billboard& operator=(Billboard&&) noexcept = default;

    void setPosition(Vector3 position);
    void setBillboardSize(float size);
    void setSource(Rectangle source);
    void setUp(Vector3 up);
    void setProSize(Vector2 size);
    void setOrigin(Vector2 origin);
    void setRotation(float rotation);
    void setTint(Color tint);

    [[nodiscard]] Vector3 position() const;
    [[nodiscard]] float billboardSize() const;
    [[nodiscard]] Rectangle source() const;
    [[nodiscard]] Vector3 up() const;
    [[nodiscard]] Vector2 proSize() const;
    [[nodiscard]] Vector2 origin() const;
    [[nodiscard]] float rotation() const;
    [[nodiscard]] Color tint() const;

    void draw(const Camera& camera) const;
    void drawPro(const Camera& camera) const;

  private:
    Rectangle _source{};
    Vector3 _position{};
    Vector3 _up{.x = 0.0F, .y = 1.0F, .z = 0.0F};
    Vector2 _size{.x = 1.0F, .y = 0.0F};
    Vector2 _origin{.x = 0.5F, .y = 0.5F};
    float _billboardSize{1.0F};
    float _rotation{0.0F};
    Color _tint{WHITE};
    bool _usePro{false};
};

class BillboardException : public std::runtime_error {
  public:
    explicit BillboardException(const std::string& message) : std::runtime_error(message) {}
};
}  // namespace zappy::gui::render
