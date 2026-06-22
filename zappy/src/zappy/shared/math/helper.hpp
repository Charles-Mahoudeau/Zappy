/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** helper
*/

#pragma once

#include <type_traits>

namespace zappy::math {
template <typename T>
concept IsArithmetic = std::is_arithmetic_v<T>;
}
