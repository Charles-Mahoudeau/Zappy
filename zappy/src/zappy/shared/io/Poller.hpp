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
    static constexpr std::byte kPollNone{0};
    static constexpr std::byte kPollError{POLLERR | POLLHUP};
    static constexpr std::byte kPollRead{POLLIN};
    static constexpr std::byte kPollWrite{POLLOUT};

    using Handler = std::function<void(std::byte events)>;

    Poller() = default;
    Poller(const Poller&) = delete;
    Poller(Poller&&) = default;
    ~Poller() = default;

    Poller& operator=(const Poller&) = delete;
    Poller& operator=(Poller&&) = default;

    void clear();
    void add(const IFileDescriptor& fileDescriptor, std::byte pollEvents, Handler callback);
    void add(int fileDescriptor, std::byte pollEvents, Handler callback);
    void remove(const IFileDescriptor& fileDescriptor);

    [[nodiscard]] std::size_t size() const;

    void poll(int32_t timeout = -1);

  private:
    struct PollEntry {
        std::byte type{0};
        Handler handler{nullptr};
    };

    std::unordered_map<int, PollEntry> _entries;
    std::vector<pollfd> _pollFds;
    std::vector<int> _toRemove;

    void reconstructPollFds();
};
}  // namespace zappy::io
