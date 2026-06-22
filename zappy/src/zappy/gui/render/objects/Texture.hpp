/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Texture
*/

#pragma once

#include <raylib.h>

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>

namespace zappy::gui::render {

class Texture {
  public:
    explicit Texture(std::string_view path, bool flipVertical = false);
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    [[nodiscard]] std::uint32_t id() const;
    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;
    [[nodiscard]] bool isValid() const;

    void reload(std::string_view path, bool flipVertical = false);
    operator Texture2D&() & { return _texture; }
    operator const Texture2D&() const& { return _texture; }
    operator Texture2D&() && = delete;
    operator const Texture2D&() const&& = delete;

    void swap(Texture& other) noexcept;
    Texture2D release() noexcept;
    static Texture2D loadTexture(const char* path, bool flipVertical = false);

  protected:
    [[nodiscard]] const Texture2D& texture() const;

  private:
    Texture2D _texture{};
};

class TextureException : public std::runtime_error {
  public:
    explicit TextureException(const std::string& message) : std::runtime_error(message) {}
};
}  // namespace zappy::gui::render
