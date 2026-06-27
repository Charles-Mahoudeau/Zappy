/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Renderer
*/

#include "Renderer.hpp"

#include <raylib.h>

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <numbers>
#include <utility>

#include "AssetStore.hpp"
#include "Camera.hpp"
#include "objects/Model.hpp"
#include "zappy/gui/display/Window.hpp"
#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/render/utils/Color.hpp"
#include "zappy/gui/render/utils/Vector3.hpp"

namespace zappy::gui::render {

void Renderer::update(Camera& camera, game::GameState& state, AssetStore& assets) {
    static constexpr float kSkyboxFovy = 60.0F;

    camera.update();

    Camera skyboxCamera{camera.position(), camera.target(), camera.up(), kSkyboxFovy,
                        CameraProjection::CAMERA_PERSPECTIVE};
    display::Window::BeginMode3D(skyboxCamera);
    assets.skybox().draw(camera.position());
    display::Window::EndMode3D();

    static float timeIncrement = 0.0F;
    timeIncrement += GetFrameTime() * static_cast<float>(state.timeUnit());
    if (timeIncrement >= 75.0F) {
        timeIncrement = 0.0F;
        assets.emit("impact", Vector3{0.0F, 0.0F, 0.0F});
        assets.emit("signal", Vector3{3.0F, 0.0F, 0.0F});
        assets.emit("signal", Vector3{6.0F, 0.0F, 0.0F});
    }
    _grid.resize(state.width(), state.height());
    display::Window::BeginMode3D(camera);
    _grid.draw(assets);
    drawResources(state, assets);
    drawEggs(state, assets);
    drawPlayers(state, assets);
    drawVFXs(camera, state, assets);
    display::Window::EndMode3D();
}

void Renderer::drawResources(const game::GameState& state, const AssetStore& assets) {
    for (std::size_t y = 0; y < state.height(); ++y) {
        for (std::size_t x = 0; x < state.width(); ++x) {
            const Vector3 position = render::Grid::tileToWorld(static_cast<float>(x), static_cast<float>(y));
            drawTileResources(state.tile(x, y), position, assets);
        }
    }
}

void Renderer::drawTileResources(const game::Resources& tile, const Vector3& position, const AssetStore& assets) {
    for (const auto& [resourceType, model] : assets.resourceModels()) {
        const std::uint32_t count = resourceCount(tile, resourceType);
        if (count == 0) {
            continue;
        }
        drawResourceStack(model, position, count, resourceType);
    }
}

void Renderer::drawResourceStack(const Model& model, const Vector3& position, std::uint32_t count,
                                 game::ResourceType type) {
    static constexpr auto kTypeCount = static_cast<float>(std::to_underlying(game::ResourceType::Thystame) + 1);
    static constexpr float kBaseRadius = 0.32F;
    static constexpr float kItemSpacing = 0.16F;

    const float angle =
        (static_cast<float>(std::to_underlying(type)) / kTypeCount) * (2.0F * std::numbers::pi_v<float>);
    const float dirX = std::cos(angle);
    const float dirZ = std::sin(angle);

    for (std::uint32_t i = 0; i < count; ++i) {
        const float radius = kBaseRadius + (static_cast<float>(i) * kItemSpacing);
        const Vector3 resourcePosition(position.x() + (dirX * radius), position.y(), position.z() + (dirZ * radius));
        model.draw(resourcePosition, kScale, Color::kWHITE);
    }
}

void Renderer::drawEggs(const game::GameState& state, const AssetStore& assets) {
    const auto& eggs = state.eggs();
    for (const auto& [eggId, egg] : eggs) {
        const Vector3 position = render::Grid::tileToWorld(static_cast<float>(egg.x), static_cast<float>(egg.y));
        assets.eggModel().draw(position, kScale, Color::kWHITE);
    }
}

std::uint32_t Renderer::resourceCount(const game::Resources& tile, game::ResourceType type) {
    using enum game::ResourceType;
    switch (type) {
        case Food:
            return tile.food;
        case Linemate:
            return tile.linemate;
        case Deraumere:
            return tile.deraumere;
        case Sibur:
            return tile.sibur;
        case Mendiane:
            return tile.mendiane;
        case Phiras:
            return tile.phiras;
        case Thystame:
            return tile.thystame;
    }
    return 0;
}

void Renderer::drawPlayers(const game::GameState& state, AssetStore& assets) {
    const auto& teams = state.teams();
    const auto& players = state.players();
    const float dt = GetFrameTime() * static_cast<float>(state.timeUnit());

    std::erase_if(_playerVisuals, [&players](const auto& entry) { return !players.contains(entry.first); });

    for (const auto& [playerId, player] : players) {
        const auto teamIt = std::ranges::find(teams, player.team);
        const auto teamIndex = static_cast<std::size_t>(std::distance(teams.begin(), teamIt));
        const std::size_t modelCount = assets.playerModelCount();
        if (modelCount == 0) {
            continue;
        }
        auto& model = assets.playerModel(teamIndex % modelCount);
        const float scale = kScale * static_cast<float>(player.level);
        const Color tint = Color{Color::teamTint(teamIndex, modelCount)};

        PlayerVisual& visual = _playerVisuals[playerId];
        updatePlayerVisual(visual, player, dt, model);

        model.drawEx(visual.position, Vector3(0.0F, 1.0F, 0.0F), calculAngle(player.orientation), scale, tint);
    }
}

float Renderer::torusNearest(float current, float target, float size) {
    float best = target;
    float bestDist = std::fabs(target - current);
    for (const float offset : {-size, size}) {
        const float candidate = target + offset;
        const float dist = std::fabs(candidate - current);
        if (dist < bestDist) {
            bestDist = dist;
            best = candidate;
        }
    }
    return best;
}

void Renderer::updatePlayerPos(PlayerVisual& visual, const game::Player& player, float dt) const {
    const Vector3 target = render::Grid::tileToWorld(static_cast<float>(player.x), static_cast<float>(player.y));
    const float worldWidth = _grid.worldWidth();
    const float worldHeight = _grid.worldHeight();

    if (!visual.initialized) {
        visual.position = target;
        visual.tileX = player.x;
        visual.tileY = player.y;
        visual.initialized = true;
    }

    if (player.x != visual.tileX || player.y != visual.tileY) {
        visual.moveStart = visual.position;
        visual.moveTarget = Vector3(torusNearest(visual.position.x(), target.x(), worldWidth), 0.0F,
                                    torusNearest(visual.position.z(), target.z(), worldHeight));
        visual.moveProgress = 0.0F;
        visual.moving = true;
        visual.tileX = player.x;
        visual.tileY = player.y;
    }

    if (visual.moving) {
        visual.moveProgress += (kMoveDuration > 0.0F) ? dt / kMoveDuration : 1.0F;
        if (visual.moveProgress >= 1.0F) {
            visual.moving = false;
            visual.position = target;
        } else {
            const float t = visual.moveProgress;
            const float posX = visual.moveStart.x() + ((visual.moveTarget.x() - visual.moveStart.x()) * t);
            const float posZ = visual.moveStart.z() + ((visual.moveTarget.z() - visual.moveStart.z()) * t);
            visual.position = Vector3(posX, 0.0F, posZ);
        }
    }
}

void Renderer::updatePlayerAnimation(PlayerVisual& visual, float dt, Model& model) {
    int desiredAnim = visual.moving ? kMoveAnim : kIdleAnim;
    if (desiredAnim >= model.animationCount()) {
        desiredAnim = kIdleAnim;
    }
    if (desiredAnim != visual.animIndex) {
        visual.animIndex = desiredAnim;
        visual.animFrame = 0.0F;
    }
    model.setCurrentAnimation(visual.animIndex);

    const int keyframeCount = model.currentAnimationKeyframeCount();
    if (keyframeCount > 0) {
        float frame = 0.0F;
        if (visual.moving) {
            frame = visual.moveProgress * static_cast<float>(keyframeCount - 1) * kWalkCycles;
        } else {
            visual.animFrame += dt * kIdleFps;
            frame = visual.animFrame;
        }
        model.setCurrentFrame(static_cast<int>(std::fmod(frame, static_cast<float>(keyframeCount))));
    }
    model.updateAnimation();
}

void Renderer::updatePlayerVisual(PlayerVisual& visual, const game::Player& player, float dt, Model& model) const {
    updatePlayerPos(visual, player, dt);
    updatePlayerAnimation(visual, dt, model);
}

float Renderer::calculAngle(game::Orientation orientation) {
    using enum game::Orientation;
    switch (orientation) {
        case South:
            return 0.0F;
        case East:
            return 90.0F;
        case North:
            return 180.0F;
        case West:
            return 270.0F;
        default:
            return 0.0F;
    }
}

void Renderer::drawVFXs(Camera& camera, game::GameState& state, AssetStore& assets) {
    uint16_t totalParticles = 0;

    for (auto& [name, emitter] : assets.vfxs()) {
        emitter.update(GetFrameTime() * static_cast<float>(state.timeUnit()));
    }
    for (auto& [name, emitter] : assets.vfxs()) {
        totalParticles += emitter.draw(camera);
        if (totalParticles >= kParticlesCeiling) {
            break;
        }
    }
}

}  // namespace zappy::gui::render
