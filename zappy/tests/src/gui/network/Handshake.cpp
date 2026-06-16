/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Handshake tests
*/

#include "zappy/gui/network/Handshake.hpp"

#include <gtest/gtest.h>

#include <type_traits>

using zappy::gui::network::Handshake;

TEST(HandshakeTest, IsMoveOnly) {
    EXPECT_FALSE(std::is_copy_constructible_v<Handshake>);
    EXPECT_FALSE(std::is_copy_assignable_v<Handshake>);
    EXPECT_TRUE(std::is_move_constructible_v<Handshake>);
    EXPECT_TRUE(std::is_move_assignable_v<Handshake>);
}
