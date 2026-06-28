/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** VFXHandler
*/

#pragma once

#include <functional>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#include "../../utils/Vector3.hpp"
#include "ParticleEmitter.hpp"

namespace zappy::gui::render {

class VFXHandler {
  public:
    VFXHandler() = default;
    VFXHandler(const VFXHandler&) = default;
    VFXHandler& operator=(const VFXHandler&) = default;
    VFXHandler(VFXHandler&&) = default;
    VFXHandler& operator=(VFXHandler&&) = default;
    ~VFXHandler() = default;

    void createVFXs();
    void emit(std::string_view path, Vector3 pos);
    void playVFX(std::vector<std::string_view>& emitters, Vector3 pos);

    [[nodiscard]] const std::map<std::string, ParticleEmitter, std::less<>>& vfxs() const;
    [[nodiscard]] std::map<std::string, ParticleEmitter, std::less<>>& vfxs();

  private:
    std::map<std::string, ParticleEmitter, std::less<>> _vfxs;
};

}  // namespace zappy::gui::render
