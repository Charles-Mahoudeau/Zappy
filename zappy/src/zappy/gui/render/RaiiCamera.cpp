/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Camera
*/

#include "RaiiCamera.hpp"

#include <raylib.h>

namespace zappy::gui::render {

RaiiCamera::RaiiCamera(const Vector3 position, const Vector3 target, const Vector3 up, const float fovy,
                       const CameraProjection projection)
    : _camera{
          .position = position, .target = target, .up = up, .fovy = fovy, .projection = static_cast<int>(projection)} {}

RaiiCamera::RaiiCamera(RaiiCamera&& other) noexcept : _camera(other._camera) { other._camera = {}; }

RaiiCamera& RaiiCamera::operator=(RaiiCamera&& other) noexcept {
    if (this != &other) {
        _camera = other._camera;
        other._camera = {};
    }
    return *this;
}

Vector3 RaiiCamera::position() const { return _camera.position; }

Vector3 RaiiCamera::target() const { return _camera.target; }

Vector3 RaiiCamera::up() const { return _camera.up; }

float RaiiCamera::fovy() const { return _camera.fovy; }

CameraProjection RaiiCamera::projection() const { return static_cast<CameraProjection>(_camera.projection); }

CameraMode RaiiCamera::cameraMode() const { return _cameraMode; }

void RaiiCamera::setPosition(const Vector3& position) { _camera.position = position; }

void RaiiCamera::setTarget(const Vector3& target) { _camera.target = target; }

void RaiiCamera::setUp(const Vector3& up) { _camera.up = up; }

void RaiiCamera::setFovy(const float fovy) { _camera.fovy = fovy; }

void RaiiCamera::setProjection(const CameraProjection projection) { _camera.projection = static_cast<int>(projection); }

void RaiiCamera::setCameraMode(const CameraMode mode) { _cameraMode = mode; }

void RaiiCamera::Update() { UpdateCamera(&_camera, static_cast<int>(_cameraMode)); }

}  // namespace zappy::gui::render
