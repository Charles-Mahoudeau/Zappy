/*
** EPITECH PROJECT, 2026
** Core
** File description:
** Core header
*/

#pragma once

#include <span>
#include <string_view>

#include "zappy/shared/network/socket/Server.hpp"

namespace zappy::server {

class Core {
  public:
    Core() = default;
    ~Core() = default;

    Core(const Core&) = delete;
    Core(Core&&) = delete;
    Core& operator=(const Core&) = delete;
    Core& operator=(Core&&) = delete;

    void init(std::span<std::string_view> argv);

  private:
    network::socket::Server _serv{};
};

}  // namespace zappy::server
