/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** StringHash
*/

#pragma once

#include <cstddef>
#include <string>
#include <string_view>

namespace zappy::helper {
class StringHash {
  public:
    using is_transparent = void;

    [[nodiscard]] std::size_t operator()(std::string_view value) const;
    [[nodiscard]] std::size_t operator()(const char* value) const;
    [[nodiscard]] std::size_t operator()(const std::string& value) const;
};
}  // namespace zappy::helper
