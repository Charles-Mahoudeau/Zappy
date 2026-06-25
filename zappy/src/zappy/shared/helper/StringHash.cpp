/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** StringHash
*/

#include "StringHash.hpp"

#include <cstddef>
#include <functional>
#include <string>
#include <string_view>

namespace zappy::helper {
std::size_t StringHash::operator()(const std::string_view value) const { return std::hash<std::string_view>{}(value); }

std::size_t StringHash::operator()(const char* value) const { return std::hash<std::string_view>{}(value); }

std::size_t StringHash::operator()(const std::string& value) const { return std::hash<std::string>{}(value); }
}  // namespace zappy::helper
