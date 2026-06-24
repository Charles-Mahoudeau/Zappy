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
#include <list>

namespace zappy::server {

class Timer {
  public:
    Timer();
    explicit Timer(std::uint16_t freq);
    ~Timer() = default;

    Timer(const Timer&) = delete;
    Timer(Timer&&) = delete;
    Timer& operator=(const Timer&) = delete;
    Timer& operator=(Timer&&) = delete;

    // return the nb tick passed since last update
    int update();

    void setFrequencies(std::uint16_t freq);

    /**
     * @brief time until the next tick. Zero if now
     * @return timeout in milliseconds
     */
    [[nodiscard]] int timeoutUntilSchedule() const;

    [[nodiscard]] int timeoutUntilNextTick() const;

    /**
     * @brief add a new event to the list
     * @param timeout time until the end of the event. time = (timeout / frequencies) seconds
     * @param notifier function which will be called at the end of the timeout. Used to notify the client that the
     *                 event is finished
     * @return id of the scheduled task
     */
    std::uint64_t scheduleLater(int timeout, std::function<void()> notifier);

    /**
     * @brief add a new event to the list and execute it every N tick
     * @param timeout time until the end of the event. time = (timeout / frequencies) seconds
     * @param notifier function which will be called at the end of the timeout. Used to notify the client that the
     *                 event is finished
     * @return id of the scheduled task
     */
    std::uint64_t scheduleEvery(int timeout, std::function<void()> notifier);

    /**
     * @brief remove the events related to the given Addr
     * @param addr @c network::Address to identify the related events
     */
    void unschedule(std::uint64_t id);

  private:
    struct Event {
        int timeout;
        std::function<void()> notifier;
        std::uint64_t id;
        int repeated_timeout = -1;
    };

    static constexpr int kTick_milli_default = 1000;
    static constexpr std::uint16_t kMinFrequency = 1;
    static constexpr std::uint16_t kMaxFrequency = 10000;
    static constexpr std::uint16_t kDefault_Frequencies = 100;

    std::uint64_t _nextId = 1;
    std::chrono::steady_clock::time_point _previousTick = std::chrono::steady_clock::now();
    std::list<Event> _events;
    std::chrono::milliseconds _tickTime{1};

    [[nodiscard]] int smallestTimeout() const;
};

}  // namespace zappy::server
