/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** CameraController
*/

#include "CameraController.hpp"

#include <raylib.h>

#include "Camera.hpp"
#include "zappy/gui/ui/Mouse.hpp"

namespace zappy::gui::render {

void CameraController::update(Camera& camera) {
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
        camera.zoom(zoomDelta);
    }
}

}  // namespace zappy::gui::render
