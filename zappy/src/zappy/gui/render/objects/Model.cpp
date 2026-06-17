/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Model
*/

#include "Model.hpp"

#include <raylib.h>

#include <cstddef>
#include <span>
#include <string>
#include <string_view>

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
    DrawModel(_model, position, scale, tint);
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

}  // namespace zappy::gui::render
