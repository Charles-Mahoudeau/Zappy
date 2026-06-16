/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Mesh
*/

#pragma once

#include <raylib.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace zappy::gui::render {
class Model {
  public:
    Model(std::string_view path);
    Model(std::string_view path, std::string_view animationPath);
    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;
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

  protected:
  private:
    ::Model _model;
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
