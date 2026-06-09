/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** InvalidArgument
*/

#pragma once

#include "Exception.hpp"

namespace zappy::exception {
class InvalidArgument : public Exception {
  public:
    using Exception::Exception;
};
}  // namespace zappy::exception
