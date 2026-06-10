/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Camera
*/

#include "RaiiCamera.hpp"

namespace zappy::gui::render {

RaiiCamera::RaiiCamera(Vector3 position, Vector3 target, Vector3 up, float fovy, CameraProjection projection) {
    _camera.position = position;
    _camera.target = target;
    _camera.up = up;
    _camera.fovy = fovy;
    _camera.projection = static_cast<int>(projection);
}

RaiiCamera::RaiiCamera(RaiiCamera&& other) noexcept {
    _camera = other._camera;
    other._camera = {0};
}

RaiiCamera& RaiiCamera::operator=(RaiiCamera&& other) noexcept {
    if (this != &other) {
        _camera = other._camera;
        other._camera = {0};
    }
    return *this;
}

Vector3 RaiiCamera::position() { return _camera.position; }

Vector3 RaiiCamera::target() { return _camera.target; }

Vector3 RaiiCamera::up() { return _camera.up; }

float RaiiCamera::fovy() { return _camera.fovy; }

CameraProjection RaiiCamera::projection() { return static_cast<CameraProjection>(_camera.projection); }

CameraMode RaiiCamera::cameraMode() { return _cameraMode; }

void RaiiCamera::setPosition(const Vector3& position) { _camera.position = position; }

void RaiiCamera::setTarget(const Vector3& target) { _camera.target = target; }

void RaiiCamera::setUp(const Vector3& up) { _camera.up = up; }

void RaiiCamera::setFovy(float fovy) { _camera.fovy = fovy; }

void RaiiCamera::setProjection(CameraProjection projection) { _camera.projection = static_cast<int>(projection); }

void RaiiCamera::setCameraMode(CameraMode mode) { _cameraMode = mode; }

void RaiiCamera::Update() { UpdateCamera(&_camera, static_cast<int>(_cameraMode)); }

}  // namespace zappy::gui::render
