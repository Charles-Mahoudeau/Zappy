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

namespace zappy::gui::render {

class Texture {
  public:
    explicit Texture(std::string_view path);
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    [[nodiscard]] std::uint32_t id() const;
    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;
    [[nodiscard]] bool isValid() const;

    void reload(std::string_view path);
    void swap(Texture& other) noexcept;

  private:
    Texture2D _texture{};
};

class TextureException : public std::runtime_error {
  public:
    explicit TextureException(const std::string& message) : std::runtime_error(message) {}
};
}  // namespace zappy::gui::render
