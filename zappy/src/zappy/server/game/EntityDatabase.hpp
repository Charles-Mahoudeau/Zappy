/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** EntityDatabase
*/

#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <ranges>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "IEntity.hpp"
#include "zappy/shared/compat/ranges.hpp"

namespace zappy::server::game {
/// @brief A database of entities.
class EntityDatabase {
  public:
    /// @brief Insert an entity into the database.
    /// @param entity The entity to insert.
    /// @return The id of the inserted entity.
    [[nodiscard]] std::uint64_t insert(std::unique_ptr<IEntity> entity);

    /// @brief Remove an entity from the database.
    /// @param id The id of the entity to remove.
    void remove(std::uint64_t id);

    /// @brief Remove all entities from the database.
    void removeAll();

    /// @brief Remove all entities of a given type from the database.
    /// @tparam T The type of the entities to remove.
    template <IsEntity T>
    void removeAll();

    /// @brief Get an entity from the database.
    /// @param id The id of the entity to get.
    /// @return A pointer to the entity, or nullptr if the entity does not exist.
    [[nodiscard]] IEntity* query(std::uint64_t id);

    /// @brief Get all entities from the database.
    /// @return A view of pointers to all entities in the database.
    auto viewAll();

    /// @brief Get all entities of a given type from the database.
    /// @tparam T The type of the entities to get.
    /// @return A view of pointers to all entities of the given type in the database.
    template <IsEntity T>
    auto viewAll();

    /// @brief Get all entities from the database as a vector.
    /// @return A vector of pointers to all entities in the database.
    [[nodiscard]] std::vector<IEntity*> toVector();

    /// @brief Get all entities of a given type from the database as a vector.
    /// @tparam T The type of the entities to get.
    /// @return A vector of pointers to all entities of the given type in the database.
    template <IsEntity T>
    [[nodiscard]] std::vector<T*> toVector();

    /// @brief Get the number of entities in the database.
    /// @return The number of entities in the database.
    [[nodiscard]] std::uint64_t countAll() const;

    /// @brief Get the number of entities of a given type in the database.
    /// @tparam T The type of the entities to count.
    /// @return The number of entities of the given type in the database.
    template <IsEntity T>
    [[nodiscard]] std::uint64_t countAll();

    /// @brief Get the ID of an entity.
    /// @param entity The entity to get the ID of.
    /// @return The ID of the entity.
    [[nodiscard]] std::optional<std::uint64_t> id(const IEntity& entity);

  private:
    /// @brief Generate a unique ID for a new entity.
    /// @return A unique ID for a new entity.
    [[nodiscard]] std::uint64_t generateId();

    /// @brief Get the type index of an entity.
    /// @param entity The entity to get the type index of.
    /// @return The type index of the entity.
    [[nodiscard]] static std::type_index typeIndex(const IEntity& entity);

    std::uint64_t _nextId{0};
    std::unordered_map<std::uint64_t, std::unique_ptr<IEntity>> _entities;
    std::unordered_map<std::type_index, std::unordered_map<std::uint64_t, std::vector<IEntity*>>> _entitiesByType;
};

template <IsEntity T>
void EntityDatabase::removeAll() {
    const auto it = _entitiesByType.find(typeid(T));

    if (it == _entitiesByType.end()) {
        return;
    }
    for (const auto& id : it->second | std::views::keys) {
        _entities.erase(id);
    }
    _entitiesByType.erase(it);
}

inline auto EntityDatabase::viewAll() {
    return _entities | std::views::values |
           std::views::transform([](const std::unique_ptr<IEntity>& entity) { return entity.get(); });
}

template <IsEntity T>
auto EntityDatabase::viewAll() {
    return _entitiesByType[typeid(T)] | std::views::values | std::views::join |
           std::views::transform([](IEntity* entity) { return static_cast<T*>(entity); });
}

template <IsEntity T>
std::vector<T*> EntityDatabase::toVector() {
    return viewAll<T>() | compat::ranges::to<std::vector<T*>>();
}

template <IsEntity T>
std::uint64_t EntityDatabase::countAll() {
    return _entitiesByType[typeid(T)].size();
}
}  // namespace zappy::server::game
