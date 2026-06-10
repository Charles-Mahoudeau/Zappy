/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Camera
*/

#include <raylib.h>

#include "Camera.hpp"

namespace zappy::gui::render {

Camera::Camera(Vector3 position, Vector3 target, Vector3 up, float fovy, CameraProjection projection)
    : _camera{
          .position = position, .target = target, .up = up, .fovy = fovy, .projection = static_cast<int>(projection)} {}

Camera::Camera(Camera&& other) noexcept : _camera(other._camera) { other._camera = {}; }

Camera& Camera::operator=(Camera&& other) noexcept {
    if (this != &other) {
        _camera = other._camera;
        other._camera = {};
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

void Camera::setFovy(const float fovy) { _camera.fovy = fovy; }

void Camera::setProjection(const CameraProjection projection) { _camera.projection = static_cast<int>(projection); }

void Camera::setCameraMode(const CameraMode mode) { _cameraMode = mode; }

void Camera::Update() { UpdateCamera(&_camera, static_cast<int>(_cameraMode)); }

}  // namespace zappy::gui::render
