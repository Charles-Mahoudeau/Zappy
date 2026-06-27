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
#include <limits>
#include <utility>

namespace zappy::server {
Timer::Timer() { this->setFrequency(kDefaultFrequency); }

Timer::Timer(std::uint16_t freq) { this->setFrequency(freq); }

int Timer::update() {
    std::chrono::steady_clock::time_point const now = std::chrono::steady_clock::now();
    const auto nbTick = static_cast<int>((now - this->_previousTick) / this->_tickTime);

    if (nbTick <= 0) {
        return 0;
    }

    this->_previousTick += this->_tickTime * nbTick;

    for (const std::uint64_t id : _toRemove) {
        std::erase_if(_events, [id](const Event& event) { return event.id == id; });
    }
    _toRemove.clear();

    auto it = this->_events.begin();

    while (it != this->_events.end()) {
        it->timeout -= nbTick;
        if (it->timeout > 0 || (it->condition != nullptr && !it->condition())) {
            ++it;
            continue;
        }
        it->notifier();
        if (it->repeatedTimeout != -1) {
            it->timeout = it->repeatedTimeout;
            ++it;
            continue;
        }
        it = this->_events.erase(it);
    }
    return nbTick;
}

std::uint16_t Timer::frequency() const { return _frequency; }

void Timer::setFrequency(std::uint16_t frequency) {
    update();
    if (frequency < kMinFrequency) {
        frequency = kMinFrequency;
    } else if (frequency > kMaxFrequency) {
        frequency = kMaxFrequency;
    }
    _frequency = frequency;
    _tickTime = std::chrono::milliseconds(kTick_milli_default) / frequency;
    _previousTick = std::chrono::steady_clock::now();
}

int Timer::timeoutUntilSchedule() const {
    if (_events.empty()) {
        return -1;
    }

    const auto now = std::chrono::steady_clock::now();
    const auto nextTicks = smallestTimeout();

    if (nextTicks == -1) {
        return -1;
    }

    const auto targetTickTime = _previousTick + nextTicks * _tickTime;
    const auto timeToWait = std::chrono::duration_cast<std::chrono::milliseconds>(targetTickTime - now);

    return timeToWait.count() <= 0 ? 0 : static_cast<int>(timeToWait.count());
}

int Timer::timeoutUntilNextTick() const {
    const auto now = std::chrono::steady_clock::now();
    const auto nextTick = _previousTick + _tickTime;
    const auto timeToWait = std::chrono::duration_cast<std::chrono::milliseconds>(nextTick - now);

    return timeToWait.count() <= 0 ? 0 : static_cast<int>(timeToWait.count());
}

std::uint64_t Timer::scheduleLater(const int timeout, std::function<void()> notifier) {
    const std::uint64_t id = _nextId;

    _events.emplace_back(Event{
        .id = id,
        .timeout = timeout,
        .notifier = std::move(notifier),
        .condition = nullptr,
        .repeatedTimeout = -1,
    });
    ++_nextId;
    return id;
}

std::uint64_t Timer::scheduleEvery(const int timeout, std::function<void()> notifier, std::function<bool()> condition) {
    const std::uint64_t id = _nextId;

    _events.emplace_back(Event{
        .id = id,
        .timeout = timeout,
        .notifier = std::move(notifier),
        .condition = std::move(condition),
        .repeatedTimeout = timeout,
    });
    ++_nextId;
    return id;
}

void Timer::unschedule(const std::uint64_t id) { _toRemove.push_back(id); }

int Timer::smallestTimeout() const {
    if (_events.empty()) {
        return -1;
    }

    int smallest = std::numeric_limits<int>::max();
    bool foundValidEvent = false;

    for (const auto& event : _events) {
        if (event.condition != nullptr && !event.condition()) {
            continue;
        }
        smallest = std::min(smallest, event.timeout);
        foundValidEvent = true;
    }
    return foundValidEvent ? smallest : -1;
}
}  // namespace zappy::server
