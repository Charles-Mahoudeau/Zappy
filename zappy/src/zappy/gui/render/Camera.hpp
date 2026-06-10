/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Camera
*/

#pragma once

#include <raylib.h>

#include <string>
#include <string_view>

namespace zappy::gui::render {

enum class CameraMode {
    CAMERA_CUSTOM = 0,
    CAMERA_FREE = 1,
    CAMERA_ORBITAL = 2,
    CAMERA_FIRST_PERSON = 3,
    CAMERA_THIRD_PERSON = 4
};

enum class CameraProjection { CAMERA_PERSPECTIVE = 0, CAMERA_ORTHOGRAPHIC = 1 };

class Camera {
  public:
    Camera(const Vector3 position, const Vector3 target, const Vector3 up, const float fovy,
           const CameraProjection projection);
    ~Camera() = default;

    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;

    Camera(Camera&& other) noexcept;
    Camera& operator=(Camera&& other) noexcept;

    Vector3 position() const;
    Vector3 target() const;
    Vector3 up() const;
    float fovy() const;
    CameraProjection projection() const;
    CameraMode cameraMode() const;

    void setPosition(const Vector3& position);
    void setTarget(const Vector3& target);
    void setUp(const Vector3& up);
    void setFovy(const float fovy);
    void setProjection(const CameraProjection projection);
    void setCameraMode(const CameraMode mode);

    void Update();

    operator Camera3D() const { return _camera; }

    operator Camera3D&() { return _camera; }
    operator const Camera3D&() const { return _camera; }

  private:
    Camera3D _camera{0};
    CameraMode _cameraMode{CameraMode::CAMERA_CUSTOM};
};
}  // namespace zappy::gui::render
