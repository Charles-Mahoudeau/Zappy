/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Renderer
*/

#pragma once

#include <cstdint>
#include <unordered_map>

#include "AssetStore.hpp"
#include "Camera.hpp"
#include "Grid.hpp"
#include "objects/Model.hpp"
#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/render/utils/Vector3.hpp"

namespace zappy::gui::render {
class Renderer {
  public:
    Renderer() = default;
    ~Renderer() = default;
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    Renderer(Renderer&& other) noexcept = default;
    Renderer& operator=(Renderer&& other) noexcept = default;

    void update(Camera& camera, game::GameState& state, AssetStore& assets);

    static void setCameraToOrthographic(Camera& camera, const game::GameState& state);
    static void setCameraToPerspective(Camera& camera, const game::GameState& state);

  private:
    static void drawResources(const game::GameState& state, const AssetStore& assets);
    static void drawTileResources(const game::Resources& tile, const Vector3& position, const AssetStore& assets);
    static void drawResourceStack(const Model& model, const Vector3& position, std::uint32_t count,
                                  game::ResourceType type);
    static void drawEggs(const game::GameState& state, const AssetStore& assets);
    static std::uint32_t resourceCount(const game::Resources& tile, game::ResourceType type);

    struct PlayerVisual {
        Vector3 position;
        Vector3 moveStart;
        Vector3 moveTarget;
        float moveProgress{0.0F};
        float animFrame{0.0F};
        int animIndex{-1};
        std::uint32_t tileX{0};
        std::uint32_t tileY{0};
        bool initialized{false};
        bool moving{false};
    };

    void drawPlayers(const game::GameState& state, AssetStore& assets);
    void updatePlayerVisual(PlayerVisual& visual, const game::Player& player, float dt, Model& model) const;
    static float torusNearest(float current, float target, float size);
    void updatePlayerPos(PlayerVisual& visual, const game::Player& player, float dt) const;
    static void updatePlayerAnimation(PlayerVisual& visual, float dt, Model& model);
    static float calculAngle(game::Orientation orientation);

    static constexpr float kScale = 0.5F;
    static constexpr float kMoveDuration = 0.8F;
    static constexpr float kIdleFps = 36.0F;
    static constexpr float kWalkCycles = 1.0F;
    static constexpr int kIdleAnim = 0;
    static constexpr int kMoveAnim = 1;
    std::unordered_map<std::uint32_t, PlayerVisual> _playerVisuals;

    static void drawVFXs(Camera& camera, game::GameState& state, AssetStore& assets);
    static constexpr uint16_t kParticlesCeiling = 1500;
    Grid _grid;
};
}  // namespace zappy::gui::render
