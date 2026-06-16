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
        Texture(const char* path);
        ~Texture();

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        Texture(Texture&& other) noexcept;
        Texture& operator=(Texture&& other) noexcept;

        [[nodiscard]] std::uint32_t id() const;
        [[nodiscard]] int width() const;
        [[nodiscard]] int height() const;

        bool isValid() const;
        bool reload(const char* path);
        void swap(Texture& other) noexcept;

    private:
        Texture2D _texture{};
};
}  // namespace zappy::gui::render
