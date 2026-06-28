/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** VFXHandler
*/

#pragma once

#include <map>
#include <string>

#include "ParticleEmitter.hpp"

namespace zappy::gui::render {

class VFXHandler {
  public:
    VFXHandler() = default;
    ~VFXHandler() = default;

    void createVFXs();
    void emit(std::string_view path, Vector3 pos);

    [[nodiscard]] const std::map<std::string, ParticleEmitter>& vfxs() const;
    [[nodiscard]] std::map<std::string, ParticleEmitter>& vfxs();

  private:
    std::map<std::string, ParticleEmitter> _vfxs;
};

}  // namespace zappy::gui::render
