/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Exception
*/

#pragma once

#include <exception>
#include <string>
#include <string_view>

namespace zappy::exception {
class Exception : public std::exception {
  public:
    explicit Exception(std::string_view message);
    Exception(const Exception&) = default;
    Exception(Exception&&) = default;
    ~Exception() override = default;

    Exception& operator=(const Exception&) = default;
    Exception& operator=(Exception&&) = default;

    [[nodiscard]] const char* what() const noexcept override;

  private:
    std::string _message;
};
}  // namespace zappy::exception
