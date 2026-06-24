/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Camera
*/

#pragma once

#include <raylib.h>

#include <cstdint>

namespace zappy::gui::render {

enum class CameraMode : std::uint8_t {
    CAMERA_CUSTOM = 0,
    CAMERA_FREE = 1,
    CAMERA_ORBITAL = 2,
    CAMERA_FIRST_PERSON = 3,
    CAMERA_THIRD_PERSON = 4
};

enum class CameraProjection : std::uint8_t { CAMERA_PERSPECTIVE = 0, CAMERA_ORTHOGRAPHIC = 1 };

class Camera {
  public:
    Camera() = default;
    Camera(Vector3 position, Vector3 target, Vector3 up, float fovy, CameraProjection projection);
    ~Camera() = default;

    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;

    Camera(Camera&& other) noexcept;
    Camera& operator=(Camera&& other) noexcept;

    [[nodiscard]] Vector3 position() const;
    [[nodiscard]] Vector3 target() const;
    [[nodiscard]] Vector3 up() const;
    [[nodiscard]] float fovy() const;
    [[nodiscard]] CameraProjection projection() const;
    [[nodiscard]] CameraMode cameraMode() const;

    void setPosition(const Vector3& position);
    void setTarget(const Vector3& target);
    void setUp(const Vector3& up);
    void setFovy(float fovy);
    void setProjection(CameraProjection projection);
    void setCameraMode(CameraMode mode);
    void zoom(float delta);

    void applyManualZoomInput();

    void update();

    operator Camera3D() const { return _camera; }

    operator Camera3D&() { return _camera; }
    operator const Camera3D&() const { return _camera; }

  private:
    Camera3D _camera{};
    CameraMode _cameraMode{CameraMode::CAMERA_CUSTOM};
};
}  // namespace zappy::gui::render
