#include "zappy/shared/math/Vector2.hpp"

#include <gtest/gtest.h>

#include "zappy/shared/exception/InvalidArgument.hpp"

TEST(Vector2Test, Constructors) {
    constexpr zappy::math::Vector2i v1;
    EXPECT_EQ(v1.x, 0);
    EXPECT_EQ(v1.y, 0);

    constexpr zappy::math::Vector2i v2{1, 2};
    EXPECT_EQ(v2.x, 1);
    EXPECT_EQ(v2.y, 2);

    constexpr zappy::math::Vector2i v3{3};
    EXPECT_EQ(v3.x, 3);
    EXPECT_EQ(v3.y, 3);
}

TEST(Vector2Test, Wrapped) {
    constexpr zappy::math::Vector2i v{10, 20};
    constexpr zappy::math::Vector2i bounds{5, 5};
    constexpr zappy::math::Vector2i result = v.wrapped(bounds);
    EXPECT_EQ(result.x, 0);
    EXPECT_EQ(result.y, 0);

    constexpr zappy::math::Vector2i v2{6, 4};
    constexpr zappy::math::Vector2i result2 = v2.wrapped(bounds);
    EXPECT_EQ(result2.x, 1);
    EXPECT_EQ(result2.y, 4);
}

TEST(Vector2Test, TypeConversion) {
    constexpr zappy::math::Vector2i v{1, 2};
    constexpr zappy::math::Vector2f vf = static_cast<zappy::math::Vector2f>(v);
    EXPECT_FLOAT_EQ(vf.x, 1.0f);
    EXPECT_FLOAT_EQ(vf.y, 2.0f);
}

TEST(Vector2Test, ArithmeticOperatorsVector) {
    constexpr zappy::math::Vector2i v1{1, 2};
    constexpr zappy::math::Vector2i v2{3, 4};

    EXPECT_EQ((v1 + v2).x, 4);
    EXPECT_EQ((v1 + v2).y, 6);

    EXPECT_EQ((v2 - v1).x, 2);
    EXPECT_EQ((v2 - v1).y, 2);

    EXPECT_EQ((v1 * v2).x, 3);
    EXPECT_EQ((v1 * v2).y, 8);

    EXPECT_EQ((v2 / v1).x, 3);
    EXPECT_EQ((v2 / v1).y, 2);
}

TEST(Vector2Test, ArithmeticOperatorsScalar) {
    constexpr zappy::math::Vector2i v{2, 4};

    EXPECT_EQ((v + 2).x, 4);
    EXPECT_EQ((v + 2).y, 6);

    EXPECT_EQ((v - 1).x, 1);
    EXPECT_EQ((v - 1).y, 3);

    EXPECT_EQ((v * 3).x, 6);
    EXPECT_EQ((v * 3).y, 12);

    EXPECT_EQ((v / 2).x, 1);
    EXPECT_EQ((v / 2).y, 2);
}

TEST(Vector2Test, ExceptionDivisionByZero) {
    constexpr zappy::math::Vector2i v1{1, 2};
    constexpr zappy::math::Vector2i v2{0, 1};

    EXPECT_THROW(v1 / v2, zappy::exception::InvalidArgument);
    EXPECT_THROW(v1 / 0, zappy::exception::InvalidArgument);
}

TEST(Vector2Test, Comparison) {
    constexpr zappy::math::Vector2i v1{1, 2};
    constexpr zappy::math::Vector2i v2{1, 2};
    constexpr zappy::math::Vector2i v3{2, 1};

    EXPECT_EQ(v1, v2);
    EXPECT_NE(v1, v3);
    EXPECT_LT(v1, v3);
}
