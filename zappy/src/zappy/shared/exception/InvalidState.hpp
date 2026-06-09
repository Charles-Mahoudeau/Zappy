/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** InvalidState
*/

#pragma once

#include "Exception.hpp"

namespace zappy::exception {
class InvalidState : public Exception {
  public:
    using Exception::Exception;
};
}  // namespace zappy::exception
