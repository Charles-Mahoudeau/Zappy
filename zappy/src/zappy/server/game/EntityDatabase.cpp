/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** EntityDatabase
*/

#include "EntityDatabase.hpp"

#include <cstdint>
#include <memory>
#include <optional>
#include <ranges>
#include <utility>
#include <vector>

#include "IEntity.hpp"

namespace zappy::server::game {
std::uint64_t EntityDatabase::insert(std::unique_ptr<IEntity> entity) {
    auto [it, _] = _entities.emplace(generateId(), std::move(entity));

    _entitiesByType[typeid(it->second.get())][it->first].emplace_back(it->second.get());
    return it->first;
}

void EntityDatabase::remove(const std::uint64_t id) {
    const auto it = _entities.find(id);

    if (it == _entities.end()) {
        return;
    }
    _entitiesByType[typeid(it->second.get())].erase(id);
    _entities.erase(it);
}

void EntityDatabase::removeAll() {
    _entities.clear();
    _entitiesByType.clear();
}

IEntity* EntityDatabase::query(const std::uint64_t id) {
    const auto it = _entities.find(id);

    if (it == _entities.end()) {
        return nullptr;
    }
    return it->second.get();
}

auto EntityDatabase::viewAll() {
    return _entities | std::views::values |
           std::views::transform([](const std::unique_ptr<IEntity>& entity) { return entity.get(); });
}

std::vector<IEntity*> EntityDatabase::toVector() { return viewAll() | std::ranges::to<std::vector<IEntity*>>(); }

std::uint64_t EntityDatabase::countAll() const { return _entities.size(); }

std::optional<std::uint64_t> EntityDatabase::id(const IEntity* entity) {
    if (entity == nullptr) {
        return std::nullopt;
    }
    for (const auto& [id, entityPtr] : _entities) {
        if (entityPtr.get() == entity) {
            return id;
        }
    }
    return std::nullopt;
}

std::optional<std::uint64_t> EntityDatabase::id(const IEntity& entity) { return id(&entity); }

std::uint64_t EntityDatabase::generateId() { return _nextId++; }
}  // namespace zappy::server::game
