/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Resource
*/

#include "Resource.hpp"

#include "zappy/server/game/ResourceType.hpp"

namespace zappy::server::game::entity {
Resource::Resource(const ResourceType type) : _type{type} {}

void Resource::update() {}

ResourceType Resource::type() const { return _type; }
}  // namespace zappy::server::game::entity
