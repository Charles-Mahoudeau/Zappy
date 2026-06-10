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

class RaiiCamera {
  public:
    RaiiCamera(Vector3 position, Vector3 target, Vector3 up, float fovy, CameraProjection projection);
    ~RaiiCamera() = default;

    RaiiCamera(const RaiiCamera&) = delete;
    RaiiCamera& operator=(const RaiiCamera&) = delete;

    RaiiCamera(RaiiCamera&& other) noexcept;
    RaiiCamera& operator=(RaiiCamera&& other) noexcept;

    Vector3 position();
    Vector3 target();
    Vector3 up();
    float fovy();
    CameraProjection projection();
    CameraMode cameraMode();

    void setPosition(const Vector3& position);
    void setTarget(const Vector3& target);
    void setUp(const Vector3& up);
    void setFovy(float fovy);
    void setProjection(CameraProjection projection);
    void setCameraMode(CameraMode mode);

    void Update();

    operator Camera() const { return _camera; }

    operator Camera&() { return _camera; }
    operator const Camera&() const { return _camera; }

  private:
    Camera _camera{0};
    CameraMode _cameraMode{CameraMode::CAMERA_CUSTOM};
};
}  // namespace zappy::gui::render
