/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** EntityDatabase
*/

#include "zappy/server/game/EntityDatabase.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include "zappy/server/game/IEntity.hpp"

namespace {
class EntityA : public zappy::server::game::IEntity {
  public:
    MOCK_METHOD(void, update, (), (override));
};  // namespace zappy::server::game::IEntity

class EntityB : public zappy::server::game::IEntity {
  public:
    MOCK_METHOD(void, update, (), (override));
};
}  // namespace

using namespace zappy::server::game;

TEST(EntityDatabase, InsertAndQuery) {
    EntityDatabase db;
    auto entity = std::make_unique<EntityA>();
    const EntityA* ptr = entity.get();
    const std::uint64_t id = db.insert(std::move(entity));

    EXPECT_EQ(db.query(id), ptr);
    EXPECT_EQ(db.countAll(), 1);
}

TEST(EntityDatabase, QueryNonExistent) {
    EntityDatabase db;

    EXPECT_EQ(db.query(42), nullptr);
}

TEST(EntityDatabase, Remove) {
    EntityDatabase db;
    const std::uint64_t id = db.insert(std::make_unique<EntityA>());

    EXPECT_EQ(db.countAll(), 1);
    db.remove(id);
    EXPECT_EQ(db.countAll(), 0);
    EXPECT_EQ(db.query(id), nullptr);
}

TEST(EntityDatabase, RemoveAll) {
    EntityDatabase db;

    std::ignore = db.insert(std::make_unique<EntityA>());
    std::ignore = db.insert(std::make_unique<EntityB>());
    EXPECT_EQ(db.countAll(), 2);
    db.removeAll();
    EXPECT_EQ(db.countAll(), 0);
}

TEST(EntityDatabase, RemoveAllByType) {
    EntityDatabase db;

    std::ignore = db.insert(std::make_unique<EntityA>());
    std::ignore = db.insert(std::make_unique<EntityA>());
    std::ignore = db.insert(std::make_unique<EntityB>());
    EXPECT_EQ(db.countAll(), 3);
    EXPECT_EQ(db.countAll<EntityA>(), 2);
    EXPECT_EQ(db.countAll<EntityB>(), 1);
    db.removeAll<EntityA>();
    EXPECT_EQ(db.countAll(), 1);
    EXPECT_EQ(db.countAll<EntityA>(), 0);
    EXPECT_EQ(db.countAll<EntityB>(), 1);
}

TEST(EntityDatabase, ViewAll) {
    EntityDatabase db;

    std::ignore = db.insert(std::make_unique<EntityA>());
    std::ignore = db.insert(std::make_unique<EntityA>());

    std::uint64_t count = 0;

    for (const auto* _ : db.viewAll()) {
        count++;
    }
    EXPECT_EQ(count, 2);
}

TEST(EntityDatabase, ViewAllByType) {
    EntityDatabase db;

    std::ignore = db.insert(std::make_unique<EntityA>());
    std::ignore = db.insert(std::make_unique<EntityA>());
    std::ignore = db.insert(std::make_unique<EntityB>());

    std::uint64_t countA = 0;

    for (auto* entity : db.viewAll<EntityA>()) {
        static_assert(std::is_same_v<decltype(entity), EntityA*>);
        countA++;
    }
    EXPECT_EQ(countA, 2);

    std::uint64_t countB = 0;

    for (auto* entity : db.viewAll<EntityB>()) {
        static_assert(std::is_same_v<decltype(entity), EntityB*>);
        countB++;
    }
    EXPECT_EQ(countB, 1);
}

TEST(EntityDatabase, ToVector) {
    EntityDatabase db;

    std::ignore = db.insert(std::make_unique<EntityA>());
    std::ignore = db.insert(std::make_unique<EntityB>());

    const auto vec = db.toVector();

    EXPECT_EQ(vec.size(), 2);
}

TEST(EntityDatabase, ToVectorByType) {
    EntityDatabase db;

    std::ignore = db.insert(std::make_unique<EntityA>());
    std::ignore = db.insert(std::make_unique<EntityA>());
    std::ignore = db.insert(std::make_unique<EntityB>());

    const std::vector<EntityA*> vecA = db.toVector<EntityA>();

    EXPECT_EQ(vecA.size(), 2);
    static_assert(std::is_same_v<std::decay_t<decltype(vecA)>, std::vector<EntityA*>>);

    const std::vector<EntityB*> vecB = db.toVector<EntityB>();

    EXPECT_EQ(vecB.size(), 1);
    static_assert(std::is_same_v<std::decay_t<decltype(vecB)>, std::vector<EntityB*>>);
}

TEST(EntityDatabase, IdLookup) {
    EntityDatabase db;
    auto entity = std::make_unique<EntityA>();
    const auto* ptr = entity.get();
    const auto id = db.insert(std::move(entity));

    EXPECT_EQ(db.id(*ptr), id);

    const EntityA other;

    EXPECT_EQ(db.id(other), std::nullopt);
}
