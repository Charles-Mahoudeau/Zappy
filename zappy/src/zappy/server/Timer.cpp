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
#include <iostream>
#include <utility>

namespace zappy::server {

Timer::Timer(std::uint16_t freq) { this->setFrequencies(freq); }

int Timer::update() {
    std::chrono::steady_clock::time_point const now = std::chrono::steady_clock::now();
    const int nbTick = static_cast<int>((now - this->_previousTick) / this->_tickTime);
    auto it = this->_events.begin();

    if (nbTick <= 0) {
        return 0;
    }

    this->_previousTick = now;

    while (it != this->_events.end()) {
        it->timeout -= nbTick;
        if (it->timeout > 0) {
            it++;
            continue;
        }
        it->notifier();
        if (it->repeated_timeout != -1) {
            it->timeout = it->repeated_timeout;
            it++;
            continue;
        }
        it = this->_events.erase(it);
    }
    return nbTick;
}

void Timer::setFrequencies(std::uint16_t freq) {
    if (freq < kMinFrequency) {
        freq = kMinFrequency;
    } else if (freq > kMaxFrequency) {
        freq = kMaxFrequency;
    }

    this->_tickTime = std::chrono::milliseconds(kTick_milli_default) / freq;
    this->_previousTick = std::chrono::steady_clock::now();
}

int Timer::timeoutUntilSchedule() {
    auto now = std::chrono::steady_clock::now();
    auto sinceTick = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->_previousTick);

    if (this->_events.empty()) {
        return -1;
    }

    int const nextTicks = this->smallestTimeout();
    int const remaining = static_cast<int>((this->_tickTime.count() * nextTicks) - sinceTick.count());

    return remaining < 0 ? 0 : remaining;
}

int Timer::timeoutUntilNextTick() {
    auto now = std::chrono::steady_clock::now();
    auto nextTick = this->_previousTick + this->_tickTime;
    auto timeToWait = std::chrono::duration_cast<std::chrono::milliseconds>(nextTick - now);

    return timeToWait.count() <= 0 ? 0 : static_cast<int>(timeToWait.count());
}

int Timer::smallestTimeout() {
    if (this->_events.empty()) {
        return 0;
    }

    int smallest = this->_events.begin()->timeout;

    for (const auto& event : this->_events) {
        smallest = std::min(event.timeout, smallest);
    }
    return smallest > 0 ? smallest : 0;
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
