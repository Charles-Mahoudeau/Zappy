/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Demo
*/

#include "zappy/shared/Demo.hpp"

#include <gtest/gtest.h>

#include "zappy/gui/Demo.hpp"
#include "zappy/server/Demo.hpp"

TEST(Demo, Hello) {
    zappy::shared::hello();
    zappy::server::hello();
    zappy::gui::hello();
}
