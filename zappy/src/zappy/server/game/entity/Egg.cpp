/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Egg
*/

#include "Egg.hpp"

#include <string>
#include <string_view>
#include <utility>

namespace zappy::server::game::entity {
Egg::Egg(std::string teamName) : _teamName{std::move(teamName)} {}

void Egg::update() {}

std::string_view Egg::teamName() const { return _teamName; }
}  // namespace zappy::server::game::entity
