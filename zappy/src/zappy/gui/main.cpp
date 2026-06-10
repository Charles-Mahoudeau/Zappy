/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** main
*/

#include <cstddef>
#include <span>
#include <string_view>
#include <vector>

#include "zappy/gui/display/Window.hpp"
#include "zappy/gui/render/Camera.hpp"

int main(const int argc, char** argv) {
    const std::span argsView{argv, static_cast<std::size_t>(argc)};
    const std::vector<std::string_view> args{argsView.begin(), argsView.end()};

    zappy::gui::display::Window window{1600, 900, "Zappy"};
    window.setTargetFPS(60);

    zappy::gui::render::Camera camera{Vector3{0, 0, 10}, Vector3{0, 0, 0}, Vector3{0, 1, 0}, 45.0f,
                                      zappy::gui::render::CameraProjection::CAMERA_ORTHOGRAPHIC};
    camera.setCameraMode(zappy::gui::render::CameraMode::CAMERA_ORBITAL);

    while (!window.shouldClose()) {
        window.beginFrame();
        camera.Update();
        window.endFrame();
    }
    return 0;
}
