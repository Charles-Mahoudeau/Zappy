/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Texture
*/

#pragma once

#include <raylib.h>

#include <cstdint>

namespace zappy::gui::render {

class Texture {
  public:
    explicit Texture(const char* path, bool flipVertical = false);
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    [[nodiscard]] std::uint32_t id() const;
    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;
    [[nodiscard]] bool isValid() const;

    operator Texture2D&() & { return _texture; }
    operator const Texture2D&() const& { return _texture; }
    operator Texture2D&() && = delete;
    operator const Texture2D&() const&& = delete;

    void reload(const char* path);
    void swap(Texture& other) noexcept;
    static Texture2D loadTexture(const char* path, bool flipVertical = false);

  private:
    Texture2D _texture{};
};
}  // namespace zappy::gui::render
