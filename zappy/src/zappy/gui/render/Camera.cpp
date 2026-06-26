/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Camera
*/

#include "Camera.hpp"

#include <raylib.h>

#include <algorithm>
#include <cmath>
#include <optional>
#include <utility>

#include "zappy/gui/render/utils/Vector3.hpp"
#include "zappy/gui/ui/Mouse.hpp"

namespace zappy::gui::render {

Camera::Camera(const Vector3 position, const Vector3 target, const Vector3 up, const float fovy,
               const CameraProjection projection)
    : _camera{
          .position = position, .target = target, .up = up, .fovy = fovy, .projection = std::to_underlying(projection)},
      _maxFovy{fovy} {}

Camera::Camera(Camera&& other) noexcept
    : _camera(other._camera),
      _cameraMode(other._cameraMode),
      _maxFovy(other._maxFovy),
      _focusInitialized(other._focusInitialized),
      _defaultTarget(other._defaultTarget),
      _manualFovy(other._manualFovy) {
    other._camera = {};
    other._cameraMode = CameraMode::CAMERA_CUSTOM;
}

Camera& Camera::operator=(Camera&& other) noexcept {
    if (this != &other) {
        _camera = other._camera;
        _cameraMode = other._cameraMode;
        _maxFovy = other._maxFovy;
        _focusInitialized = other._focusInitialized;
        _defaultTarget = other._defaultTarget;
        _manualFovy = other._manualFovy;
        other._camera = {};
        other._cameraMode = CameraMode::CAMERA_CUSTOM;
    }
    return *this;
}

Vector3 Camera::position() const { return Vector3{_camera.position}; }

Vector3 Camera::target() const { return Vector3{_camera.target}; }

Vector3 Camera::up() const { return Vector3{_camera.up}; }

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
    _camera.fovy = std::clamp(_camera.fovy - (delta * kZoomSpeed), kMinFovy, _maxFovy);
    _manualFovy = _camera.fovy;
}

void Camera::applyManualZoomInput() {
    static constexpr float kKeyZoomSpeed = 20.0F;

    float zoomDelta = ui::Mouse::scrollDelta();
    const float keyZoom = kKeyZoomSpeed * GetFrameTime();
    if (IsKeyDown(KEY_EQUAL) || IsKeyDown(KEY_KP_ADD)) {
        zoomDelta += keyZoom;
    }
    if (IsKeyDown(KEY_MINUS) || IsKeyDown(KEY_KP_SUBTRACT)) {
        zoomDelta -= keyZoom;
    }
    if (zoomDelta != 0.0F) {
        zoom(zoomDelta);
    }
}

void Camera::followPlayer(std::optional<Vector3> worldPosition) {
    static constexpr float kBlendSpeed = 4.0F;
    static constexpr float kFocusFovyFactor = 0.35F;

    if (!_focusInitialized) {
        _defaultTarget = target();
        _manualFovy = fovy();
        _focusInitialized = true;
    }

    if (!worldPosition.has_value()) {
        applyManualZoomInput();
    }

    const float blend = std::clamp(kBlendSpeed * GetFrameTime(), 0.0F, 1.0F);
    const Vector3 targetGoal = worldPosition.value_or(_defaultTarget);
    const float fovyGoal = worldPosition.has_value() ? (_manualFovy * kFocusFovyFactor) : _manualFovy;

    const Vector3 currentTarget = target();
    setTarget(Vector3{std::lerp(currentTarget.x(), targetGoal.x(), blend), 0.0F,
                      std::lerp(currentTarget.z(), targetGoal.z(), blend)});
    setFovy(std::lerp(fovy(), fovyGoal, blend));
}

void Camera::update() { UpdateCamera(&_camera, std::to_underlying(_cameraMode)); }

}  // namespace zappy::gui::render
