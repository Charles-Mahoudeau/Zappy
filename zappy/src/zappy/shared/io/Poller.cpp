/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Poller
*/

#include "Poller.hpp"

#include <sys/poll.h>

#include <algorithm>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <ranges>
#include <system_error>
#include <utility>

#include "IFileDescriptor.hpp"
#include "zappy/shared/exception/InvalidArgument.hpp"
#include "zappy/shared/exception/InvalidState.hpp"

namespace zappy::io {
void Poller::clear() {
    _entries.clear();
    _pollFds.clear();
    _toRemove.clear();
}

void Poller::add(const IFileDescriptor& fileDescriptor, const std::byte pollEvents, Handler callback) {
    if (callback == nullptr) {
        throw exception::InvalidArgument{"callback cannot be null"};
    }
    _entries.insert_or_assign(fileDescriptor.fd(), PollEntry{
                                                       .type = pollEvents,
                                                       .handler = std::move(callback),
                                                   });
    reconstructPollFds();
}

void Poller::add(const int fileDescriptor, const std::byte pollEvents, Handler callback) {
    if (callback == nullptr) {
        throw exception::InvalidArgument{"callback cannot be null"};
    }
    _entries.insert_or_assign(fileDescriptor, PollEntry{
                                                  .type = pollEvents,
                                                  .handler = std::move(callback),
                                              });
    reconstructPollFds();
}

void Poller::remove(const IFileDescriptor& fileDescriptor) { _toRemove.push_back(fileDescriptor.fd()); }

void Poller::remove(const int fd) { _toRemove.push_back(fd); }

std::size_t Poller::size() const { return _entries.size(); }

void Poller::poll(const std::int32_t timeout) {
    if (_entries.size() != _pollFds.size()) {
        throw exception::InvalidState{"Poller _pollFds and _fds size mismatch"};
    }

    const auto removed = std::erase_if(
        _entries, [this](const auto& fd) { return std::ranges::find(_toRemove, fd.first) != _toRemove.end(); });

    if (!_toRemove.empty()) {
        _toRemove.clear();
    }
    if (removed > 0) {
        reconstructPollFds();
    }

    const int ret = ::poll(_pollFds.data(), _entries.size(), timeout);

    if (ret == -1) {
        if (errno == EINTR) {
            return;
        }

        const std::error_code error{errno, std::generic_category()};

        throw exception::InvalidState{"Failed to poll: " + error.message()};
    }
    if (ret == 0) {
        return;
    }
    for (std::size_t i = 0; i < _entries.size(); i++) {
        const pollfd& pollEntry = _pollFds.at(i);
        const PollEntry& entry = _entries.at(pollEntry.fd);

        const auto hasEvent = [&pollEntry, &entry](const std::byte event) {
            return (static_cast<std::byte>(pollEntry.revents) & event) != std::byte{0} &&
                   (entry.type & event) != std::byte{0};
        };

        std::byte events{0};

        if (hasEvent(kPollError)) {
            events |= kPollError;
        }
        if (hasEvent(kPollRead)) {
            events |= kPollRead;
        }
        if (hasEvent(kPollWrite)) {
            events |= kPollWrite;
        }
        if (events != std::byte{0}) {
            entry.handler(events);
        }
    }
}

void Poller::reconstructPollFds() {
    _pollFds.clear();
    for (const auto& [fd, entry] : _entries) {
        _pollFds.emplace_back(pollfd{
            .fd = fd,
            .events = std::to_underlying(entry.type),
            .revents = 0,
        });
    }
}
}  // namespace zappy::io
