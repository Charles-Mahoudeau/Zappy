/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** SocketError
*/

#pragma once

#include "Exception.hpp"

namespace zappy::exception {
class SocketError : public Exception {
  public:
    using Exception::Exception;
};
}  // namespace zappy::exception
