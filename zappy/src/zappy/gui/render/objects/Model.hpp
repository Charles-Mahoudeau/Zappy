/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Mesh
*/

#pragma once

#include <raylib.h>

#include <stdexcept>
#include <string>
#include <string_view>

namespace zappy::gui::render {
class Model {
  public:
    enum class MaterialMapIndex {
        MATERIAL_MAP_ALBEDO = 0,     // Albedo material (alias: MATERIAL_MAP_DIFFUSE)
        MATERIAL_MAP_METALNESS = 1,  // Metalness material (alias: MATERIAL_MAP_SPECULAR)
        MATERIAL_MAP_NORMAL,         // Normal material
        MATERIAL_MAP_ROUGHNESS,      // Roughness material
        MATERIAL_MAP_OCCLUSION,      // Ambient occlusion material
        MATERIAL_MAP_EMISSION,       // Emission material
        MATERIAL_MAP_HEIGHT,         // Heightmap material
        MATERIAL_MAP_CUBEMAP,        // Cubemap material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
        MATERIAL_MAP_IRRADIANCE,     // Irradiance material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
        MATERIAL_MAP_PREFILTER,      // Prefilter material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
        MATERIAL_MAP_BRDF            // Brdf material
    };

    Model(std::string_view path);
    Model(std::string_view path, std::string_view animationPath);
    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;
    Model(Model&& other) noexcept;
    Model& operator=(Model&& other) noexcept;
    ~Model();

    void draw(Vector3 position, float scale, Color tint) const;
    void updateAnimation();

    void setCurrentAnimation(int anim) { _currentAnim = anim; }
    void setCurrentFrame(int frame) { _currentFrame = frame; }

    void addAnimationFrame() { ++_currentFrame; }
    void subtractAnimationFrame() { --_currentFrame; }
    void resetAnimationFrame() { _currentFrame = 0; }

    void addAnimationCount() { ++_animCount; }
    void subtractAnimationCount() { --_animCount; }
    void resetAnimationCount() { _animCount = 0; }

    void setTexture(int materialIndex, MaterialMapIndex mapIndex, ::Texture texture);
    void setMeshTexture(int meshIndex, MaterialMapIndex mapIndex, ::Texture texture);

  protected:
  private:
    ::Model _model{};
    ModelAnimation* _animations;
    int _animCount;
    int _currentAnim;
    int _currentFrame;
};

class ModelException : public std::runtime_error {
  public:
    explicit ModelException(const std::string& message) : std::runtime_error(message) {}
};
}  // namespace zappy::gui::render
