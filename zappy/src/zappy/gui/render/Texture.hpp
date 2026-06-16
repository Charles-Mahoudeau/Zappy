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

        bool isValid() const { return _texture.id != 0; }
        bool reload(const char* path) {
            UnloadTexture(_texture);
            _texture = LoadTexture(path);
            return isValid();
        }
        void swap(Texture& other) noexcept {
            Texture2D tmp = _texture;

            _texture = other._texture;
            other._texture = tmp;
        }

    private:
        Texture2D _texture{};
};
}  // namespace zappy::gui::render
