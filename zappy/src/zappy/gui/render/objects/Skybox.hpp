/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Skybox
*/

#pragma once

#include <raylib.h>

#include <stdexcept>
#include <string>
#include <string_view>

namespace zappy::gui::render {

class SkyboxException : public std::runtime_error {
  public:
    using std::runtime_error::runtime_error;
};

class Skybox {
  public:
    Skybox() = default;
    explicit Skybox(std::string_view path, std::string_view shadersDirectory = "assets/shaders", bool hdr = false);
    ~Skybox();

    Skybox(const Skybox&) = delete;
    Skybox& operator=(const Skybox&) = delete;

    Skybox(Skybox&& other) noexcept;
    Skybox& operator=(Skybox&& other) noexcept;

    void draw(::Vector3 center) const;

  private:
    static constexpr int kCubemapSize = 1024;
    static constexpr float kCubeSize = 30.0F;

    static std::string shadersPath(std::string_view shadersDirectory);
    static ::TextureCubemap loadCubemapImage(std::string_view path);
    static ::TextureCubemap loadPanoramaCubemap(std::string_view path, const std::string& shadersDirectory);
    static ::TextureCubemap generateCubemap(::Shader shader, ::Texture2D panorama, int size);

    ::Model _model{};
    ::Shader _shader{};
    ::TextureCubemap _cubemap{};
};

}  // namespace zappy::gui::render
