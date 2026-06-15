/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Poller
*/

#pragma once

#include <sys/poll.h>

#include <cstddef>
#include <cstdint>
#include <functional>
#include <unordered_map>
#include <vector>

namespace zappy::io {
class IFileDescriptor;

class Poller {
  public:
    static constexpr std::uint8_t kPollNone = 0;
    static constexpr std::uint8_t kPollError = POLLERR | POLLHUP;
    static constexpr std::uint8_t kPollRead = POLLIN;
    static constexpr std::uint8_t kPollWrite = POLLOUT;

    using Handler = std::function<void(std::uint8_t events)>;

    Poller() = default;
    Poller(const Poller&) = delete;
    Poller(Poller&&) = default;
    ~Poller() = default;

    Poller& operator=(const Poller&) = delete;
    Poller& operator=(Poller&&) = default;

    void clear();
    void add(const IFileDescriptor& fileDescriptor, std::uint8_t pollEvents, Handler callback);
    void add(int fileDescriptor, std::uint8_t pollEvents, Handler callback);
    void remove(const IFileDescriptor& fileDescriptor);

    [[nodiscard]] std::size_t size() const;

    void poll();

  private:
    struct PollEntry {
        std::uint8_t type{0};
        Handler handler{nullptr};
    };

    std::unordered_map<int, PollEntry> _entries;
    std::vector<pollfd> _pollFds;
    std::vector<int> _toRemove;

    void reconstructPollFds();
};
}  // namespace zappy::io
