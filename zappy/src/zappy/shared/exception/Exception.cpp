/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Exception
*/

#include "Exception.hpp"

#include <string>
#include <string_view>

namespace zappy::exception {
Exception::Exception(const std::string_view message) : _message{message} {}

const char* Exception::what() const noexcept { return _message.c_str(); }
}  // namespace zappy::exception
