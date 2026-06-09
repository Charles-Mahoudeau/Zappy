/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** InvalidAddress
*/

#pragma once

#include "Exception.hpp"

namespace zappy::exception {
class InvalidAddress : public Exception {
  public:
    using Exception::Exception;
};
}  // namespace zappy::exception
