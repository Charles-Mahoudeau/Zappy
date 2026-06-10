/*
** EPITECH PROJECT, 2026
** Core
** File description:
** Core header
*/

#pragma once

#include <string_view>
#include <vector>

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

    void init(const std::vector<std::string_view>& argv);

  private:
    network::socket::Server _serv{};
};

}  // namespace zappy::server
