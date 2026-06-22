/*
** EPITECH PROJECT, 2026
** Timer
** File description:
** Timer header
*/

#pragma once

#include <chrono>
#include <cstdint>
#include <functional>
#include <queue>
#include <vector>

#include "zappy/shared/network/Address.hpp"

namespace zappy::server {

class Timer {
  public:
    Timer() = default;
    ~Timer() = default;

    Timer(const Timer&) = delete;
    Timer(Timer&&) = delete;
    Timer& operator=(const Timer&) = delete;
    Timer& operator=(Timer&&) = delete;

    /**
     * @brief set the frequencies of the timer
     * @param freq value of the frequencies
     */
    void init(std::uint16_t freq);

    /**
     * @brief compute the poll's timeout from the current time to the next request
     * @return timeout in milliseconds
     */
    int pollTimeOut();

    /**
     * @brief add a new event to the list
     * @param timeout time until the end of the event. time = (timeout / frequencies) seconds
     * @param addr @c network::Address used to identify which client the event is related to
     * @param notifier function which will be called at the end of the timeout. Used to notify the client that the
     *                 event is finished
     */
    void addEvent(int timeout, network::Address addr, std::function<void()> notifier);

    /**
     * @brief remove the events related to the given Addr
     * @param addr @c network::Address to identify the related events
     */
    void removeByAddr(network::Address& addr);

  private:
    struct Event {
        std::chrono::steady_clock::time_point timeout;
        std::function<void()> notifier;
        network::Address addr;

        bool operator>(const Event& other) const { return this->timeout > other.timeout; }
    };
    std::uint8_t _freq = 100;
    std::priority_queue<Event, std::vector<Event>, std::greater<>> _queue;

    // Members
};

}  // namespace zappy::server
