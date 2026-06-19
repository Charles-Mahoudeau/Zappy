/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** MockServerHelpers
*/

#pragma once

#include <algorithm>
#include <cstddef>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#include "zappy/shared/io/Poller.hpp"
#include "zappy/shared/network/socket/Client.hpp"

namespace zappy::test {

constexpr int kMockServerReadTimeoutMs = 5000;

inline void serverSend(zappy::network::socket::Client& client, std::string_view data) {
    auto bytes = std::as_bytes(std::span{data});
    if (bytes.empty()) {
        return;
    }
    std::size_t sent = 0;
    while (sent < bytes.size()) {
        sent += client.send(bytes.subspan(sent));
    }
}

// Waits for the socket to become readable before calling the blocking
// read(), so a peer that never sends anything causes a clear failure
// instead of hanging the test suite indefinitely.
inline std::string serverRead(zappy::network::socket::Client& client, int timeoutMs = kMockServerReadTimeoutMs) {
    zappy::io::Poller poller;
    bool ready = false;
    poller.add(client.fd(), zappy::io::Poller::kPollRead | zappy::io::Poller::kPollError,
               [&ready](std::byte /*events*/) { ready = true; });
    poller.poll(timeoutMs);
    if (!ready) {
        throw std::runtime_error{"serverRead: timed out waiting for data"};
    }

    auto bytes = client.read(4096);
    if (bytes.empty()) {
        return {};
    }
    std::string result(bytes.size(), '\0');
    std::ranges::transform(bytes, result.begin(), [](std::byte b) { return static_cast<char>(std::to_underlying(b)); });
    return result;
}

}  // namespace zappy::test
