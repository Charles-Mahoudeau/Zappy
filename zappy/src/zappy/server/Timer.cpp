/*
** EPITECH PROJECT, 2026
** Timer
** File description:
** Timer code
*/

#include "zappy/server/Timer.hpp"

#include <chrono>
#include <cstdint>
#include <functional>
#include <utility>
#include <vector>

#include "zappy/shared/network/Address.hpp"

namespace zappy::server {

void Timer::init(std::uint16_t freq) { this->_freq = freq; }

int Timer::pollTimeOut() {
    auto clock = std::chrono::steady_clock::now();

    while (!this->_queue.empty() && this->_queue.top().timeout < clock) {
        this->_queue.top().notifier();
        this->_queue.pop();
    }
    if (this->_queue.empty()) {
        return -1;
    }
    auto next = this->_queue.top();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(next.timeout - clock).count();
    return static_cast<int>(ms);
}

void Timer::addEvent(int timeout, network::Address addr, std::function<void()> notifier) {
    auto now = std::chrono::steady_clock::now();
    const double seconds = static_cast<double>(timeout) / this->_freq;
    auto deadline =
        now + std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<double>(seconds));

    this->_queue.emplace(deadline, std::move(notifier), addr);
}

void Timer::removeByAddr(network::Address& addr) {
    std::vector<Event> temp;

    while (!this->_queue.empty()) {
        if (this->_queue.top().addr != addr) {
            temp.emplace_back(this->_queue.top());
        }
        this->_queue.pop();
    }

    for (auto& e : temp) {
        this->_queue.push(std::move(e));
    }
}

}  // namespace zappy::server
