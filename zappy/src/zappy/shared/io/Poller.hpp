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
    ~Poller() = default;

    Poller(const Poller&) = delete;
    Poller& operator=(const Poller&) = delete;

    Poller(Poller&&) = default;
    Poller& operator=(Poller&&) = default;

    /// @brief Clears all file descriptors from the poller.
    void clear();

    /// @brief Adds a file descriptor to the poller.
    /// @param fileDescriptor The file descriptor to add.
    /// @param pollEvents The events to poll for.
    /// @param callback The callback to invoke when events occur.
    void add(const IFileDescriptor& fileDescriptor, std::byte pollEvents, Handler callback);

    /// @brief Adds a file descriptor to the poller.
    /// @param fileDescriptor The file descriptor to add.
    /// @param pollEvents The events to poll for.
    /// @param callback The callback to invoke when events occur.
    void add(int fileDescriptor, std::byte pollEvents, Handler callback);

    /// @brief Removes a file descriptor from the poller.
    /// @param fileDescriptor The file descriptor to remove.
    void remove(const IFileDescriptor& fileDescriptor);

    /// @brief Removes a file descriptor from the poller.
    /// @param fd The file descriptor to remove.
    void remove(int fd);

    /// @brief Retrieves the number of file descriptors currently in the poller.
    /// @return The number of file descriptors.
    [[nodiscard]] std::size_t size() const;

    /// @brief Polls for events on the file descriptors.
    /// @param timeout The timeout in milliseconds, or -1 for infinite.
    void poll(int32_t timeout = -1);

  private:
    struct PollEntry {
        std::byte type{0};
        Handler handler{nullptr};
    };

    std::unordered_map<int, PollEntry> _entries;
    std::vector<pollfd> _pollFds;
    std::vector<int> _toRemove;

    /// @brief Reconstructs the poll file descriptors based on the current entries.
    void reconstructPollFds();
};
}  // namespace zappy::io
