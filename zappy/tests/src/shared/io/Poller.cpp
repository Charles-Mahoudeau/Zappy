#include "zappy/shared/io/Poller.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <unistd.h>

#include <array>
#include <cstddef>

#include "zappy/shared/io/IFileDescriptor.hpp"

namespace {
class MockFileDescriptor : public zappy::io::IFileDescriptor {
  public:
    MOCK_METHOD(int, fd, (), (const, override));
};
}  // namespace

namespace zappy::io {
TEST(PollerTest, BasicFunctionality) {
    Poller poller;
    EXPECT_EQ(poller.size(), 0);

    poller.add(10, Poller::kPollRead, [](std::byte) {
        // Poller callback
    });
    EXPECT_EQ(poller.size(), 1);

    poller.add(11, Poller::kPollRead, [](std::byte) {
        // Poller callback
    });
    EXPECT_EQ(poller.size(), 2);

    poller.clear();
    EXPECT_EQ(poller.size(), 0);
}

TEST(PollerTest, RemoveFunctionality) {
    Poller poller;
    const MockFileDescriptor mockFd;
    EXPECT_CALL(mockFd, fd()).WillRepeatedly(testing::Return(10));

    poller.add(mockFd, Poller::kPollRead, [](std::byte) {
        // Poller callback
    });
    EXPECT_EQ(poller.size(), 1);

    poller.remove(mockFd);
    // Note: _toRemove is cleared only during poll().
    // So size() might still be 1 until poll() is called.
    EXPECT_EQ(poller.size(), 1);

    poller.poll(0);
    EXPECT_EQ(poller.size(), 0);
}

TEST(PollerTest, ReadFunctionality) {
    Poller poller;
    std::array<int, 2> pipeFds{};

    ASSERT_EQ(pipe(pipeFds.data()), 0);

    const int readFd = pipeFds.at(0);
    const int writeFd = pipeFds.at(1);
    bool callbackCalled = false;

    poller.add(readFd, Poller::kPollRead, [&callbackCalled](std::byte events) {
        if ((events & Poller::kPollRead) != std::byte{0}) {
            callbackCalled = true;
        }
    });

    // Verify callback not called initially
    poller.poll(0);
    EXPECT_FALSE(callbackCalled);

    // Write to pipe
    constexpr char buffer = 'a';
    EXPECT_EQ(write(writeFd, &buffer, 1), 1);

    // Verify callback called
    poller.poll(0);
    EXPECT_TRUE(callbackCalled);

    close(readFd);
    close(writeFd);
}
}  // namespace zappy::io
