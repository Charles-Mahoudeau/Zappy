/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Mesh
*/

#pragma once

#include <raylib.h>

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>

namespace zappy::gui::render {
class Model {
  public:
    enum class MaterialMapIndex : std::uint8_t {
        ALBEDO = 0,      // Albedo material (alias: DIFFUSE)
        METALNESS = 1,   // Metalness material (alias: SPECULAR)
        NORMAL = 2,      // Normal material
        ROUGHNESS = 3,   // Roughness material
        OCCLUSION = 4,   // Ambient occlusion material
        EMISSION = 5,    // Emission material
        HEIGHT = 6,      // Heightmap material
        CUBEMAP = 7,     // Cubemap material
        IRRADIANCE = 8,  // Irradiance material
        PREFILTER = 9,   // Prefilter material
        BRDF = 10        // Brdf material
    };

    Model(std::string_view path);
    Model(std::string_view path, std::string_view animationPath);
    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;
    Model(Model&& other) noexcept;
    Model& operator=(Model&& other) noexcept;
    ~Model();

    void draw(Vector3 position, float scale, Color tint) const;
    void drawEx(Vector3 position, Vector3 rotationAxis, float rotationAngle, Vector3 scale, Color tint) const;
    void drawEx(Vector3 position, Vector3 rotationAxis, float rotationAngle, float scale, Color tint) const;
    void normalizeOnGround(float targetSize = 1.0F);

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
    float _baseScale{1.0F};
    Vector3 _groundOffset{};
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
