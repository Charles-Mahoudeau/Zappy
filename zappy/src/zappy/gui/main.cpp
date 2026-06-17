/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** main
*/

#include <raylib.h>

#include <cstddef>
#include <span>
#include <string_view>
#include <vector>

#include "zappy/gui/display/Window.hpp"
#include "zappy/gui/render/Camera.hpp"
#include "zappy/gui/render/objects/Model.hpp"

int main(const int argc, char** argv) {
    const std::span argsView{argv, static_cast<std::size_t>(argc)};
    const std::vector<std::string_view> args{argsView.begin(), argsView.end()};

    zappy::gui::display::Window window{1600, 900, "Zappy"};
    window.setTargetFPS(60);

    zappy::gui::render::Camera camera{Vector3{.x = 0, .y = 0, .z = 10}, Vector3{.x = 0, .y = 0, .z = 0},
                                      Vector3{.x = 0, .y = 1, .z = 0}, 45.0F,
                                      zappy::gui::render::CameraProjection::CAMERA_PERSPECTIVE};
    camera.setCameraMode(zappy::gui::render::CameraMode::CAMERA_FREE);
    const zappy::gui::render::Model model{"assets/models/burpy/source/burpy.glb"};

    while (!window.shouldClose()) {
        window.beginFrame();
        camera.Update();
        BeginMode3D(camera);

        model.draw(Vector3{.x = 0, .y = 0, .z = 0}, 10.0F, WHITE);

        EndMode3D();
        window.endFrame();
    }
    return 0;
}
