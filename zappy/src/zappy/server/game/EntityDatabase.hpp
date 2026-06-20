/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** EntityDatabase
*/

#pragma once

#include <algorithm>
#include <cstdint>
#include <memory>
#include <optional>
#include <ranges>
#include <span>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "IEntity.hpp"

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

    /// @brief Get an entity of a given type from the database.
    /// @tparam T The type of the entity to get.
    /// @param id The id of the entity to get.
    /// @return A pointer to the entity, or nullptr if the entity does not exist or if the entity is not of the given
    /// type.
    template <IsEntity T>
    [[nodiscard]] T* query(std::uint64_t id);

    /// @brief Get all entities from the database.
    /// @return A view of pointers to all entities in the database.
    auto viewAll() const;

    /// @brief Get all entities from the database.
    /// @return A view of pointers to all entities in the database.
    auto viewAll();

    /// @brief Get all entities of a given type from the database.
    /// @tparam T The type of the entities to get.
    /// @return A view of pointers to all entities of the given type in the database.
    template <IsEntity T>
    auto viewAll() const;

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
    [[nodiscard]] std::uint64_t countAll() const;

    /// @brief Get the ID of an entity.
    /// @param entity The entity to get the ID of.
    /// @return The ID of the entity.
    [[nodiscard]] std::optional<std::uint64_t> id(const IEntity& entity);

    /// @brief Get the entities of a given type from a list of IDs.
    /// @tparam T The type of the entities to get.
    /// @param ids The list of IDs to get the entities from.
    /// @return A vector of all entity ids of the given type in the database.
    template <IsEntity T>
    auto filter(std::span<std::uint64_t> ids) const;

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
    std::unordered_map<std::type_index, std::unordered_map<std::uint64_t, IEntity*>> _entitiesByType;
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

template <IsEntity T>
T* EntityDatabase::query(const std::uint64_t id) {
    IEntity* entity = query(id);

    if (entity == nullptr) {
        return nullptr;
    }
    if (typeid(T) != typeIndex(*entity)) {
        return nullptr;
    }
    return static_cast<T*>(entity);
}

inline auto EntityDatabase::viewAll() const {
    return _entities | std::views::values |
           std::views::transform([](const std::unique_ptr<IEntity>& entity) { return entity.get(); });
}

inline auto EntityDatabase::viewAll() {
    return _entities | std::views::values |
           std::views::transform([](const std::unique_ptr<IEntity>& entity) { return entity.get(); });
}

template <IsEntity T>
auto EntityDatabase::viewAll() const {
    const auto it = _entitiesByType.find(typeid(T));
    static const std::remove_cvref_t<decltype(it->second)> emptyMap;
    const auto& entities = it != _entitiesByType.end() ? it->second : emptyMap;

    return entities | std::views::values |
           std::views::transform([](IEntity* entity) { return static_cast<T*>(entity); });
}

template <IsEntity T>
auto EntityDatabase::viewAll() {
    const auto it = _entitiesByType.find(typeid(T));
    static const std::remove_cvref_t<decltype(it->second)> emptyMap;
    const auto& entities = it != _entitiesByType.end() ? it->second : emptyMap;

    return entities | std::views::values |
           std::views::transform([](IEntity* entity) { return static_cast<T*>(entity); });
}

template <IsEntity T>
std::vector<T*> EntityDatabase::toVector() {
    std::vector<T*> result;

    result.reserve(countAll<T>());
    for (const auto& entity : viewAll<T>()) {
        result.emplace_back(entity);
    }
    return result;
}

template <IsEntity T>
std::uint64_t EntityDatabase::countAll() const {
    const auto it = _entitiesByType.find(typeid(T));

    return (it != _entitiesByType.end()) ? it->second.size() : 0;
}

template <IsEntity T>
auto EntityDatabase::filter(std::span<std::uint64_t> ids) const {
    const auto it = _entitiesByType.find(typeid(T));
    static const std::remove_cvref_t<decltype(it->second)> emptyMap;
    const auto& entities = it != _entitiesByType.end() ? it->second : emptyMap;

    return entities |
           std::views::filter([ids](const auto& pair) { return std::ranges::find(ids, pair.first) != ids.end(); }) |
           std::views::keys;
}
}  // namespace zappy::server::game
