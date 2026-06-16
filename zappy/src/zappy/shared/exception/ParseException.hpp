/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** ParseException
*/

#pragma once

#include "Exception.hpp"

namespace zappy::exception {
class ParseException : public Exception {
  public:
    using Exception::Exception;
};
}  // namespace zappy::exception
