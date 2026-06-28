/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** VFXHandler
*/

#include "VFXHandler.hpp"

#include <map>
#include <string>

#include "ParticleEmitter.hpp"

namespace zappy::gui::render {

void VFXHandler::createVFXs() {
    ParticleEmitter impact("assets/particles/Impact.png");
    impact.setStatic(Vector3{}, Vector3{0.5F, 0.5F, 0.5F}, Vector3{0.0F, 0.0F, 0.0F}, 0.0F, 1);
    impact.setInitParticles(Vector2{0.5F, 0.5F}, 0.0F, ColorF{180.0F, 180.0F, 180.0F, 180.0F}, 45.0F, 0.0F);
    impact.setInitEnvelope(ParticleEmitter::ParticleEnvelope{.size = 0.3F, .rotation = 360.0F, .tint = 0.3F}, 0.1F,
                           0.0F);
    impact.setIncrementParticles(Vector2{0.045F, 0.045F}, 0.0F, ColorF{-4.0F, -4.0F, -4.0F, -4.0F});
    impact.setIncrementEnvelope(ParticleEmitter::ParticleEnvelope{.size = 0.01F, .rotation = 0.02F, .tint = 0.1F});
    _vfxs.insert({"impact", std::move(impact)});

    ParticleEmitter signal("assets/particles/Shockwave.png");
    signal.setStatic(Vector3{}, Vector3{0.0F, 0.0F, 0.0F}, Vector3{0.0F, 0.0F, 0.0F}, 0.0F, 1);
    signal.setInitParticles(Vector2{1.0F, 1.0F}, 0.0F, ColorF{220.0F, 255.0F, 255.0F, 255.0F}, 600.0F, 0.0F);
    signal.setInitEnvelope(ParticleEmitter::ParticleEnvelope{.size = 0.3F, .rotation = 0.0F, .tint = 0.3F}, 0.1F, 0.0F);
    signal.setIncrementParticles(Vector2{0.08F, 0.08F}, 0.0F, ColorF{-1.2F, -1.2F, -1.2F, -1.2F});
    signal.setIncrementEnvelope(ParticleEmitter::ParticleEnvelope{.size = 0.01F, .rotation = 0.0F, .tint = 0.1F});
    _vfxs.insert({"signal", std::move(signal)});

    ParticleEmitter eggCrack("assets/particles/Shard.png");
    eggCrack.setStatic(Vector3{}, Vector3{1.0F, 1.0F, 1.0F}, Vector3{0.0F, -0.002F, 0.0F}, 100.0F, 16);
    eggCrack.setInitParticles(Vector2{0.8F, 0.8F}, 0.0F, ColorF{245.0F, 220.0F, 180.0F, 255.0F}, 90.0F, 0.06F);
    eggCrack.setInitEnvelope(ParticleEmitter::ParticleEnvelope{.size = 0.2F, .rotation = 360.0F, .tint = 30.0F}, 3.0F,
                             0.03F);
    eggCrack.setIncrementParticles(Vector2{-0.01F, -0.01F}, 0.0F, ColorF{-1.0F, -1.0F, -1.0F, -1.0F});
    eggCrack.setIncrementEnvelope(ParticleEmitter::ParticleEnvelope{.size = 0.002F, .rotation = 3.0F, .tint = 0.1F});
    _vfxs.insert({"eggCrack", std::move(eggCrack)});

    ParticleEmitter foodDebris("assets/particles/Shard.png");
    foodDebris.setStatic(Vector3{}, Vector3{1.5F, 1.5F, 1.5F}, Vector3{0.0F, -0.001F, 0.0F}, 100.0F, 14);
    foodDebris.setInitParticles(Vector2{0.4F, 0.4F}, 0.0F, ColorF{255.0F, 170.0F, 170.0F, 255.0F}, 90.0F, 0.003F);
    foodDebris.setInitEnvelope(ParticleEmitter::ParticleEnvelope{.size = 0.2F, .rotation = 360.0F, .tint = 50.0F}, 3.0F,
                               0.03F);
    foodDebris.setIncrementParticles(Vector2{-0.007F, -0.007F}, 0.0F, ColorF{-1.0F, -1.0F, -1.0F, -1.0F});
    foodDebris.setIncrementEnvelope(ParticleEmitter::ParticleEnvelope{.size = 0.002F, .rotation = 3.0F, .tint = 0.1F});
    _vfxs.insert({"foodDebris", std::move(foodDebris)});

    ParticleEmitter smoke("assets/particles/Smoke.png");
    smoke.setStatic(Vector3{}, Vector3{3.0F, 3.0F, 3.0F}, Vector3{0.0F, 0.0002F, 0.0F}, 180.0F, 16);
    smoke.setInitParticles(Vector2{1.2F, 1.2F}, 0.0F, ColorF{60.0F, 55.0F, 50.0F, 50.0F}, 200.0F, 0.03F);
    smoke.setInitEnvelope(ParticleEmitter::ParticleEnvelope{.size = 0.2F, .rotation = 360.0F, .tint = 10.0F}, 0.1F,
                          0.03F);
    smoke.setIncrementParticles(Vector2{0.01F, 0.01F}, 0.0F, ColorF{-0.5F, -0.5F, -0.5F, -0.75F});
    smoke.setIncrementEnvelope(ParticleEmitter::ParticleEnvelope{.size = 0.004F, .rotation = 20.0F, .tint = 0.3F});
    _vfxs.insert({"smoke", std::move(smoke)});

    ParticleEmitter sparkles("assets/particles/Twinkle.png");
    sparkles.setStatic(Vector3{}, Vector3{3.0F, 3.5F, 3.0F}, Vector3{0.0F, 0.00003F, 0.0F}, 180.0F, 14);
    sparkles.setInitParticles(Vector2{0.5F, 0.5F}, 45.0F, ColorF{255.0F, 130.0F, 255.0F, 50.0F}, 130.0F, 0.001F);
    sparkles.setInitEnvelope(ParticleEmitter::ParticleEnvelope{.size = 0.3F, .rotation = 0.0F, .tint = 15.0F}, 0.1F,
                             0.001F);
    sparkles.setIncrementParticles(Vector2{-0.007F, -0.007F}, 0.0F, ColorF{-1.0F, -1.0F, -1.0F, -2.0F});
    sparkles.setIncrementEnvelope(ParticleEmitter::ParticleEnvelope{.size = 0.005F, .rotation = 0.002F, .tint = 0.25F});
    _vfxs.insert({"sparkles", std::move(sparkles)});

    ParticleEmitter skull("assets/particles/Skull.png");
    skull.setStatic(Vector3{}, Vector3{0.5F, 0.5F, 0.5F}, Vector3{0.0F, -0.00009F, 0.0F}, 0.0F, 1);
    skull.setInitParticles(Vector2{2.5F, 1.2F}, 0.0F, ColorF{255.0F, 120.0F, 120.0F, 255.0F}, 150.0F, 0.013F);
    skull.setInitEnvelope(ParticleEmitter::ParticleEnvelope{.size = 0.05F, .rotation = 0.0F, .tint = 0.1F}, 0.1F,
                          0.001F);
    skull.setIncrementParticles(Vector2{-0.005F, 0.008F}, 0.0F, ColorF{-1.5F, -1.7F, -1.7F, -2.0F});
    skull.setIncrementEnvelope(ParticleEmitter::ParticleEnvelope{.size = 0.01F, .rotation = 0.00F, .tint = 0.0F});
    _vfxs.insert({"skull", std::move(skull)});

    ParticleEmitter slash("assets/particles/Slash.png");
    slash.setStatic(Vector3{}, Vector3{2.0F, 4.0F, 2.0F}, Vector3{0.0F, 0.0001F, 0.0F}, 0.0F, 12);
    slash.setInitParticles(Vector2{0.7F, 0.3F}, 0.0F, ColorF{150.0F, 255.0F, 220.0F, 50.0F}, 150.0F, 0.0002F);
    slash.setInitEnvelope(ParticleEmitter::ParticleEnvelope{.size = 0.0F, .rotation = 0.0F, .tint = 35.0F}, 0.1F,
                          0.00002F);
    slash.setIncrementParticles(Vector2{0.001F, 0.012F}, 0.0F, ColorF{-2.2F, -2.2F, -2.2F, -2.5F});
    slash.setIncrementEnvelope(ParticleEmitter::ParticleEnvelope{.size = 0.004F, .rotation = 0.0F, .tint = 1.0F});
    _vfxs.insert({"slash", std::move(slash)});
}

const std::map<std::string, ParticleEmitter>& VFXHandler::vfxs() const { return _vfxs; }

std::map<std::string, ParticleEmitter>& VFXHandler::vfxs() { return _vfxs; }

void VFXHandler::emit(std::string_view path, Vector3 pos) {
    auto it = _vfxs.find(std::string{path});

    if (it != _vfxs.end()) {
        ParticleEmitter& emitter = it->second;
        emitter.setOrigin(pos);
        emitter.emitRate();
    }
}
}  // namespace zappy::gui::render
