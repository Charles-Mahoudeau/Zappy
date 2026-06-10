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
#include <typeindex>
#include <utility>

#include "IEntity.hpp"
#include "zappy/shared/exception/InvalidArgument.hpp"

namespace zappy::server::game {
std::uint64_t EntityDatabase::insert(std::unique_ptr<IEntity> entity) {
    if (entity == nullptr) {
        throw exception::InvalidArgument{"Entity cannot be null"};
    }

    auto [it, _] = _entities.emplace(generateId(), std::move(entity));

    _entitiesByType[typeIndex(*it->second)][it->first].emplace_back(it->second.get());
    return it->first;
}

void EntityDatabase::remove(const std::uint64_t id) {
    const auto it = _entities.find(id);

    if (it == _entities.end()) {
        return;
    }
    _entitiesByType[typeIndex(*it->second)].erase(id);
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

std::uint64_t EntityDatabase::countAll() const { return _entities.size(); }

std::optional<std::uint64_t> EntityDatabase::id(const IEntity& entity) {
    for (const auto& [id, entityPtr] : _entities) {
        if (entityPtr.get() == &entity) {
            return id;
        }
    }
    return std::nullopt;
}

std::uint64_t EntityDatabase::generateId() { return _nextId++; }

std::type_index EntityDatabase::typeIndex(const IEntity& entity) { return typeid(entity); }
}  // namespace zappy::server::game
