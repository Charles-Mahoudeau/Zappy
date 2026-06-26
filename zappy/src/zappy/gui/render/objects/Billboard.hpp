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
#include "zappy/gui/render/utils/Color.hpp"
#include "zappy/gui/render/utils/Rectangle.hpp"
#include "zappy/gui/render/utils/Vector2.hpp"
#include "zappy/gui/render/utils/Vector3.hpp"

namespace zappy::gui::render {
class Camera;
}

namespace zappy::gui::render {

class Billboard : public Texture {
  public:
    explicit Billboard(std::string_view path);
    ~Billboard() = default;

    Billboard(const Billboard&) = delete;
    Billboard& operator=(const Billboard&) = delete;
    Billboard(Billboard&&) noexcept = default;
    Billboard& operator=(Billboard&&) noexcept = default;

    void setSimpleBillboard(Vector3 position, float size = 1.0F, Color tint = Color{WHITE});
    void setBillboard(Vector3 position, Vector2 size = {1.0F, 1.0F}, float rotation = 0.0F, Color tint = Color{WHITE},
                      Vector3 up = {0.0F, 1.0F, 0.0F});
    void setPosition(Vector3 position);
    void setBillboardSize(float size);
    void setSource(Rectangle source);
    void setUp(Vector3 up);
    void setSize(Vector2 size);
    void setOrigin(Vector2 origin);
    void setRotation(float rotation);
    void setTint(Color tint);

    [[nodiscard]] Vector3 position() const;
    [[nodiscard]] float billboardSize() const;
    [[nodiscard]] Rectangle source() const;
    [[nodiscard]] Vector3 up() const;
    [[nodiscard]] Vector2 size() const;
    [[nodiscard]] Vector2 origin() const;
    [[nodiscard]] float rotation() const;
    [[nodiscard]] Color tint() const;

    void draw(Camera& camera) const;  // Depreciated, only for very simple billboards, use drawPro for more control
    void drawPro(Camera& camera) const;

  private:
    Rectangle _source;
    Vector3 _position{0.0F, 0.0F, 0.0F};
    Vector3 _up{0.0F, 1.0F, 0.0F};
    Vector2 _size{1.0F, 1.0F};
    Vector2 _origin{0.5F, 0.5F};
    float _rotation{0.0F};
    Color _tint{Color::kWHITE};
};

class BillboardException : public std::runtime_error {
  public:
    explicit BillboardException(const std::string& message) : std::runtime_error(message) {}
};
}  // namespace zappy::gui::render
