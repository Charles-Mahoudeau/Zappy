#include "zappy/shared/io/Poller.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

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

    poller.add(10, Poller::kPollRead, [](std::byte) {});
    EXPECT_EQ(poller.size(), 1);

    poller.add(11, Poller::kPollRead, [](std::byte) {});
    EXPECT_EQ(poller.size(), 2);

    poller.clear();
    EXPECT_EQ(poller.size(), 0);
}

TEST(PollerTest, RemoveFunctionality) {
    Poller poller;
    const MockFileDescriptor mockFd;
    EXPECT_CALL(mockFd, fd()).WillRepeatedly(testing::Return(10));

    poller.add(mockFd, Poller::kPollRead, [](std::byte) {});
    EXPECT_EQ(poller.size(), 1);

    poller.remove(mockFd);
    // Note: _toRemove is cleared only during poll().
    // So size() might still be 1 until poll() is called.
    EXPECT_EQ(poller.size(), 1);

    poller.poll(0);
    EXPECT_EQ(poller.size(), 0);
}
}  // namespace zappy::io
