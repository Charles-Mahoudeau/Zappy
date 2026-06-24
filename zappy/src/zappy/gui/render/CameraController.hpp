/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** CameraController
*/

#pragma once

namespace zappy::gui::render {

class Camera;

class CameraController {
  public:
    CameraController() = default;
    ~CameraController() = default;

    CameraController(const CameraController&) = delete;
    CameraController& operator=(const CameraController&) = delete;

    CameraController(CameraController&&) noexcept = default;
    CameraController& operator=(CameraController&&) noexcept = default;

    static void update(Camera& camera);
};

}  // namespace zappy::gui::render
