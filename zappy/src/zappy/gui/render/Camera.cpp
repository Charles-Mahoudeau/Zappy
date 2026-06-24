/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Camera
*/

#include "Camera.hpp"

#include <raylib.h>

#include <algorithm>
#include <utility>

namespace zappy::gui::render {

Camera::Camera(const Vector3 position, const Vector3 target, const Vector3 up, const float fovy,
               const CameraProjection projection)
    : _camera{.position = position,
              .target = target,
              .up = up,
              .fovy = fovy,
              .projection = std::to_underlying(projection)} {}

Camera::Camera(Camera&& other) noexcept : _camera(other._camera), _cameraMode(other._cameraMode) {
    other._camera = {};
    other._cameraMode = CameraMode::CAMERA_CUSTOM;
}

Camera& Camera::operator=(Camera&& other) noexcept {
    if (this != &other) {
        _camera = other._camera;
        _cameraMode = other._cameraMode;
        other._camera = {};
        other._cameraMode = CameraMode::CAMERA_CUSTOM;
    }
    return *this;
}

Vector3 Camera::position() const { return _camera.position; }

Vector3 Camera::target() const { return _camera.target; }

Vector3 Camera::up() const { return _camera.up; }

float Camera::fovy() const { return _camera.fovy; }

CameraProjection Camera::projection() const { return static_cast<CameraProjection>(_camera.projection); }

CameraMode Camera::cameraMode() const { return _cameraMode; }

void Camera::setPosition(const Vector3& position) { _camera.position = position; }

void Camera::setTarget(const Vector3& target) { _camera.target = target; }

void Camera::setUp(const Vector3& up) { _camera.up = up; }

void Camera::setFovy(float fovy) { _camera.fovy = fovy; }

void Camera::setProjection(CameraProjection projection) { _camera.projection = std::to_underlying(projection); }

void Camera::setCameraMode(CameraMode mode) { _cameraMode = mode; }

void Camera::zoom(float delta) {
    static constexpr float kZoomSpeed = 2.0F;
    static constexpr float kMinFovy = 5.0F;
    static constexpr float kMaxFovy = 500.0F;
    _camera.fovy = std::clamp(_camera.fovy - (delta * kZoomSpeed), kMinFovy, kMaxFovy);
}

void Camera::update() { UpdateCamera(&_camera, std::to_underlying(_cameraMode)); }

}  // namespace zappy::gui::render
