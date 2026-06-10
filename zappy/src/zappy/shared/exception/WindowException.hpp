/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** WindowException
*/

#pragma once

#include "Exception.hpp"

namespace zappy::exception {
class WindowException : public Exception {
  public:
    using Exception::Exception;
};
}  // namespace zappy::exception
