/*
** EPITECH PROJECT, 2026
** Timer
** File description:
** Timer tests
*/

#include "zappy/server/Timer.hpp"

#include <gtest/gtest.h>

#include <chrono>
#include <cstdint>
#include <thread>

#include "gmock/gmock.h"

using namespace zappy::server;
using namespace std::chrono_literals;

namespace {

void sleepForTicks(int ticks, int freq) {
    auto perTick = std::chrono::milliseconds(1000) / freq;
    std::this_thread::sleep_for(perTick * ticks);
}

// ---------------------------------------------------------------------------
// update() / tick counting
// ---------------------------------------------------------------------------

TEST(TimerUpdate, ReturnsElapsedTicks) {
    Timer timer(150);

    sleepForTicks(3, 150);
    const int ticks = timer.update();

    ASSERT_EQ(ticks, 3);
}

TEST(TimerUpdate, ReturnsZeroWhenNoTickElapsed) {
    Timer timer;

    timer.update();
    const int ticks = timer.update();

    ASSERT_EQ(ticks, 0);
}

// ---------------------------------------------------------------------------
// scheduleLater()
// ---------------------------------------------------------------------------

TEST(TimerScheduleLater, FiresAfterTimeout) {
    Timer timer(100);
    int count = 0;

    timer.scheduleLater(2, [&count]() { count++; });

    sleepForTicks(2, 100);
    timer.update();

    EXPECT_EQ(count, 1);
}

TEST(TimerScheduleLater, DoesNotFireBeforeTimeout) {
    Timer timer(100);  // 10ms per tick
    int count = 0;

    timer.scheduleLater(5, [&count]() { count++; });

    sleepForTicks(1, 100);
    timer.update();

    EXPECT_EQ(count, 0);
}

TEST(TimerScheduleLater, FiresOnlyOnce) {
    Timer timer(150);
    int count = 0;

    timer.scheduleLater(1, [&count]() { count++; });

    sleepForTicks(1, 150);
    timer.update();
    sleepForTicks(2, 150);
    timer.update();

    EXPECT_EQ(count, 1);
}

TEST(TimerScheduleLater, ReturnsUniqueIds) {
    Timer timer(100);

    std::uint64_t id1 = timer.scheduleLater(1, []() { /* need no logic */ });
    std::uint64_t id2 = timer.scheduleLater(1, []() { /* need no logic */ });

    EXPECT_NE(id1, id2);
}

// ---------------------------------------------------------------------------
// scheduleEvery()
// ---------------------------------------------------------------------------

TEST(TimerScheduleEvery, FiresRepeatedly) {
    Timer timer(150);
    int count = 0;

    timer.scheduleEvery(2, [&count]() { count++; });

    for (std::uint8_t index = 0; index < 3; index++) {
        sleepForTicks(2, 150);
        timer.update();
    }

    EXPECT_EQ(count, 3);
}

// ---------------------------------------------------------------------------
// unschedule()
// ---------------------------------------------------------------------------

TEST(TimerUnschedule, RemovesPendingEvent) {
    Timer timer(100);
    int count = 0;

    std::uint64_t id = timer.scheduleLater(2, [&count]() { count++; });
    timer.unschedule(id);

    sleepForTicks(3, 100);
    timer.update();

    EXPECT_EQ(count, 0);
}

TEST(TimerUnschedule, RemovesRepeatingEvent) {
    Timer timer(100);
    int count = 0;

    std::uint64_t id = timer.scheduleEvery(1, [&count]() { count++; });
    timer.unschedule(id);

    sleepForTicks(2, 100);
    timer.update();

    EXPECT_EQ(count, 0);
}

TEST(TimerUnschedule, UnknownIdDoesNotAffectOthers) {
    Timer timer(100);
    int count = 0;

    std::uint64_t id = timer.scheduleLater(1, [&count]() { count++; });
    timer.unschedule(id + 999);

    sleepForTicks(1, 100);
    timer.update();

    EXPECT_EQ(count, 1);
}

// ---------------------------------------------------------------------------
// timeoutUntilSchedule()
// ---------------------------------------------------------------------------

TEST(TimerTimeoutUntilSchedule, NoEventsReturnsMinusOne) {
    const Timer timer(100);

    EXPECT_EQ(timer.timeoutUntilSchedule(), -1);
}

TEST(TimerTimeoutUntilSchedule, ReflectsSoonestEvent) {
    Timer timer(100);

    timer.scheduleLater(10, []() { /* need no logic */ });
    timer.scheduleLater(2, []() { /* need no logic */ });

    const int remaining = timer.timeoutUntilSchedule();

    EXPECT_EQ(remaining, 20);
}

// ---------------------------------------------------------------------------
// timeoutUntilNextTick()
// ---------------------------------------------------------------------------

TEST(TimerTimeoutUntilNextTick, WithinTickBounds) {
    const Timer freshTimer(10);

    const int time = freshTimer.timeoutUntilNextTick();

    EXPECT_THAT(time, testing::AllOf(testing::Ge(98), testing::Le(100)));
}

TEST(TimerTimeoutUntilNextTick, ReturnsZeroWhenDue) {
    const Timer timer(100);

    sleepForTicks(2, 100);

    EXPECT_EQ(timer.timeoutUntilNextTick(), 0);
}

// ---------------------------------------------------------------------------
// init() / setFrequencies() effect on tick duration
// ---------------------------------------------------------------------------

TEST(TimerInit, HigherFrequencyProducesMoreTicks) {
    Timer slow;
    slow.setFrequencies(100);

    Timer fast;
    fast.setFrequencies(150);

    std::this_thread::sleep_for(20ms);

    const int slowTicks = slow.update();
    const int fastTicks = fast.update();

    EXPECT_GT(fastTicks, slowTicks);
}

TEST(TimerSetFrequencies, ChangesTickDuration) {
    Timer timer(100);

    sleepForTicks(2, 100);

    int ticks = timer.update();
    EXPECT_EQ(ticks, 2);
    timer.setFrequencies(200);

    sleepForTicks(3, 200);
    ticks = timer.update();

    EXPECT_EQ(ticks, 3);
}

}  // namespace
