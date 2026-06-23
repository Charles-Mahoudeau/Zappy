/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** OutOfRange
*/

#pragma once

#include "Exception.hpp"

namespace zappy::exception {
class OutOfRange : public Exception {
  public:
    using Exception::Exception;
};
}  // namespace zappy::exception
