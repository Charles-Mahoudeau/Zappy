
#include <sys/socket.h>
#include <unistd.h>

#include <array>
struct SocketPair {
    int local = -1;
    int peer = -1;

    SocketPair() {
        std::array<int, 2> fds{};
        if (::socketpair(AF_UNIX, SOCK_STREAM, 0, fds.data()) == 0) {
            local = fds.at(0);
            peer = fds.at(1);
        }
    }
    ~SocketPair() {
        if (local != -1) {
            ::close(local);
        }
        if (peer != -1) {
            ::close(peer);
        }
    }

    SocketPair(const SocketPair&) = default;
    SocketPair(SocketPair&&) = delete;
    SocketPair& operator=(const SocketPair&) = default;
    SocketPair& operator=(SocketPair&&) = delete;
};
