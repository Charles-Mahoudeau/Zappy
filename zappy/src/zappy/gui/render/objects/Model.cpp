/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Model
*/

#include "Model.hpp"

#include <raylib.h>
#include <raymath.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <span>
#include <string>
#include <string_view>
#include <utility>

namespace zappy::gui::render {
Model::Model(std::string_view path)
    : _model(LoadModel(std::string(path).c_str())),
      _animCount(0),
      _currentAnim(0),
      _currentFrame(0),
      _animations(nullptr) {
    if (!IsModelValid(_model)) {
        throw ModelException{"Failed to load mesh from path: " + std::string{path}};
    }
}

Model::Model(std::string_view path, std::string_view animationPath)
    : _model(LoadModel(std::string(path).c_str())), _animCount(0), _currentAnim(0), _currentFrame(0) {
    if (!IsModelValid(_model)) {
        throw ModelException{"Failed to load mesh from path: " + std::string{path}};
    }

    try {
        _animations = LoadModelAnimations(std::string(animationPath).c_str(), &_animCount);
        if (_animations == nullptr || _animCount <= 0) {
            throw ModelException{"Failed to load animation from path: " + std::string{animationPath}};
        }
    } catch (const ModelException& e) {
        if (_animations != nullptr && _animCount > 0) {
            UnloadModelAnimations(_animations, _animCount);
        }
        if (IsModelValid(_model)) {
            UnloadModel(_model);
        }
        throw;
    }
}

Model::~Model() {
    if (IsModelValid(_model)) {
        UnloadModel(_model);
    }
    if (_animations != nullptr && _animCount > 0) {
        UnloadModelAnimations(_animations, _animCount);
    }
    _animations = nullptr;
    _animCount = 0;
}

Model::Model(Model&& other) noexcept
    : _model(other._model),
      _baseScale(other._baseScale),
      _groundOffset(other._groundOffset),
      _animations(other._animations),
      _animCount(other._animCount),
      _currentAnim(other._currentAnim),
      _currentFrame(other._currentFrame) {
    other._model = {};
    other._animations = nullptr;
    other._animCount = 0;
}

Model& Model::operator=(Model&& other) noexcept {
    if (this != &other) {
        if (IsModelValid(_model)) {
            UnloadModel(_model);
        }
        if (_animations != nullptr && _animCount > 0) {
            UnloadModelAnimations(_animations, _animCount);
        }
        _model = other._model;
        _baseScale = other._baseScale;
        _groundOffset = other._groundOffset;
        _animations = other._animations;
        _animCount = other._animCount;
        _currentAnim = other._currentAnim;
        _currentFrame = other._currentFrame;
        other._model = {};
        other._animations = nullptr;
        other._animCount = 0;
    }
    return *this;
}
void Model::draw(Vector3 position, float scale, Color tint) const {
    if (!IsModelValid(_model)) {
        throw ModelException{"Cannot draw a model that failed to load"};
    }
    const float finalScale = scale * _baseScale;
    const Vector3 groundedPosition = Vector3Subtract(position, Vector3Scale(_groundOffset, finalScale));
    DrawModel(_model, groundedPosition, finalScale, tint);
}

void Model::drawEx(Vector3 position, Vector3 rotationAxis, float rotationAngle, Vector3 scale, Color tint) const {
    if (!IsModelValid(_model)) {
        throw ModelException{"Cannot draw a model that failed to load"};
    }
    const Vector3 finalScale = Vector3Scale(scale, _baseScale);
    const Vector3 scaledOffset = Vector3Multiply(_groundOffset, finalScale);
    const Vector3 rotatedOffset = Vector3RotateByAxisAngle(scaledOffset, rotationAxis, rotationAngle * DEG2RAD);
    const Vector3 groundedPosition = Vector3Subtract(position, rotatedOffset);
    DrawModelEx(_model, groundedPosition, rotationAxis, rotationAngle, finalScale, tint);
}

void Model::drawEx(Vector3 position, Vector3 rotationAxis, float rotationAngle, float scale, Color tint) const {
    drawEx(position, rotationAxis, rotationAngle, Vector3{.x = scale, .y = scale, .z = scale}, tint);
}

void Model::normalizeOnGround(float targetSize) {
    if (!IsModelValid(_model)) {
        return;
    }
    const BoundingBox local = GetModelBoundingBox(_model);
    const std::array<Vector3, 8> corners = {{{.x = local.min.x, .y = local.min.y, .z = local.min.z},
                                             {.x = local.max.x, .y = local.min.y, .z = local.min.z},
                                             {.x = local.min.x, .y = local.max.y, .z = local.min.z},
                                             {.x = local.min.x, .y = local.min.y, .z = local.max.z},
                                             {.x = local.max.x, .y = local.max.y, .z = local.min.z},
                                             {.x = local.max.x, .y = local.min.y, .z = local.max.z},
                                             {.x = local.min.x, .y = local.max.y, .z = local.max.z},
                                             {.x = local.max.x, .y = local.max.y, .z = local.max.z}}};
    Vector3 mn = Vector3Transform(corners.at(0), _model.transform);
    Vector3 mx = mn;
    for (const auto& corner : corners) {
        const Vector3 point = Vector3Transform(corner, _model.transform);
        mn = Vector3{.x = std::min(mn.x, point.x), .y = std::min(mn.y, point.y), .z = std::min(mn.z, point.z)};
        mx = Vector3{.x = std::max(mx.x, point.x), .y = std::max(mx.y, point.y), .z = std::max(mx.z, point.z)};
    }
    const float maxDim = std::max({mx.x - mn.x, mx.y - mn.y, mx.z - mn.z});
    _baseScale = (maxDim > 0.0F) ? targetSize / maxDim : 1.0F;
    _groundOffset = Vector3{.x = (mn.x + mx.x) * 0.5F, .y = mn.y, .z = (mn.z + mx.z) * 0.5F};
}

void Model::updateAnimation() {
    if (!IsModelValid(_model)) {
        throw ModelException{"Cannot update animation for a model that failed to load"};
    }
    const std::span<ModelAnimation> animations{_animations, static_cast<std::size_t>(_animCount)};
    if (_currentAnim < 0 || static_cast<std::size_t>(_currentAnim) >= animations.size()) {
        return;
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)
    if (_currentFrame < 0 || _currentFrame >= animations[_currentAnim].keyframeCount) {
        _currentFrame = 0;
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)
    UpdateModelAnimation(_model, animations[_currentAnim], static_cast<float>(_currentFrame));
}

void Model::setTexture(int materialIndex, MaterialMapIndex mapIndex, ::Texture texture) {
    static constexpr std::size_t kMaterialMapCount = std::to_underlying(MaterialMapIndex::BRDF) + 1;

    if (!IsModelValid(_model)) {
        throw ModelException{"Cannot set texture for a model that failed to load"};
    }
    if (materialIndex < 0 || materialIndex >= _model.materialCount) {
        throw ModelException{"Material index out of bounds"};
    }
    const auto map = std::to_underlying(mapIndex);
    if (map >= kMaterialMapCount) {
        throw ModelException{"Material map index out of bounds"};
    }
    const std::span<Material> materials{_model.materials, static_cast<std::size_t>(_model.materialCount)};
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)
    const std::span<MaterialMap> maps{materials[static_cast<std::size_t>(materialIndex)].maps, kMaterialMapCount};
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)
    maps[map].texture = texture;
}

void Model::setMeshTexture(int meshIndex, MaterialMapIndex mapIndex, ::Texture texture) {
    if (!IsModelValid(_model)) {
        throw ModelException{"Cannot set texture for a model that failed to load"};
    }
    if (meshIndex < 0 || meshIndex >= _model.meshCount) {
        throw ModelException{"Mesh index out of bounds"};
    }
    const std::span<int> meshMaterial{_model.meshMaterial, static_cast<std::size_t>(_model.meshCount)};
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)
    setTexture(meshMaterial[meshIndex], mapIndex, texture);
}

}  // namespace zappy::gui::render
