/*
** EPITECH PROJECT, 2026
** Timer
** File description:
** Timer code
*/

#include "zappy/server/Timer.hpp"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <functional>
#include <utility>

namespace zappy::server {

void Timer::init(std::uint16_t freq) { this->_tickTime = std::chrono::milliseconds(kTick_milli_default) / freq; }

void Timer::update() {
    std::chrono::steady_clock::time_point const now = std::chrono::steady_clock::now();
    const int nbTick = static_cast<int>((now - this->_previousTick).count() / this->_tickTime.count());

    auto it = this->_events.begin();

    while (it != this->_events.end()) {
        it->timeout -= nbTick;
        if (it->timeout > 0) {
            continue;
        }
        it->notifier();
        if (it->repeated_timeout != -1) {
            it->timeout = it->repeated_timeout;
            continue;
        }
        it = this->_events.erase(it);
    }
}

void Timer::setFrequencies(int freq) { this->_tickTime = std::chrono::milliseconds(kTick_milli_default) / freq; }

int Timer::timeout() {
    std::chrono::steady_clock::time_point const now = std::chrono::steady_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(now - (this->_previousTick + this->_tickTime));
    int timeout = static_cast<int>(time.count());

    if (!this->_events.empty()) {
        const int nextTimeout = this->smallestTimeout();
        timeout += static_cast<int>(this->_tickTime.count() * (nextTimeout - 1));
    }

    if (timeout < 0) {
        return 0;
    }

    return timeout;
}

int Timer::smallestTimeout() {
    int smallest = this->_events.begin()->timeout;

    for (const auto& event : this->_events) {
        smallest = std::min(event.timeout, smallest);
    }
    return smallest;
}

void Timer::unschedule(std::uint64_t id) {
    std::erase_if(this->_events, [id](const Event& event) { return event.id == id; });
}

std::uint64_t Timer::scheduleLater(int timeout, std::function<void()> notifier) {
    std::uint64_t id = this->_nextId;
    this->_events.emplace_back(timeout, std::move(notifier), id);
    this->_nextId++;
    return id;
}

std::uint64_t Timer::scheduleEvery(int timeout, std::function<void()> notifier) {
    std::uint64_t id = this->_nextId;
    this->_events.emplace_back(timeout, std::move(notifier), id, timeout);
    this->_nextId++;
    return id;
}

}  // namespace zappy::server
