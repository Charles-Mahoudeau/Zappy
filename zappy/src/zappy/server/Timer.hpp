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
#include <vector>

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

    /// @brief Returns the frequency of the timer.
    /// @return The frequency of the timer.
    [[nodiscard]] std::uint16_t frequency() const;

    /// @brief Set the frequency of the timer.
    /// @param frequency The new frequency of the timer.
    void setFrequency(std::uint16_t frequency);

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
     * @param condition The condition to check before scheduling the event.
     * @return id of the scheduled task
     */
    std::uint64_t scheduleEvery(int timeout, std::function<void()> notifier, std::function<bool()> condition = nullptr);

    /**
     * @brief remove the events related to the given Addr
     * @param id The id of the event to unschedule
     */
    void unschedule(std::uint64_t id);

  private:
    struct Event {
        std::uint64_t id;
        int timeout;
        std::function<void()> notifier;
        std::function<bool()> condition;
        int repeatedTimeout = -1;
    };

    static constexpr int kTick_milli_default = 1000;
    static constexpr std::uint16_t kMinFrequency = 1;
    static constexpr std::uint16_t kMaxFrequency = 1000;
    static constexpr std::uint16_t kDefaultFrequency = 100;

    [[nodiscard]] int smallestTimeout() const;
    [[nodiscard]] bool isEventWaitingRemoval(std::uint64_t eventId) const;
    void applyEventsRemoval();

    std::uint64_t _nextId = 1;
    std::chrono::steady_clock::time_point _previousTick = std::chrono::steady_clock::now();
    std::list<Event> _events;
    std::uint16_t _frequency{kDefaultFrequency};
    std::chrono::milliseconds _tickTime{1};
    std::vector<std::uint64_t> _toRemove;
};

}  // namespace zappy::server
