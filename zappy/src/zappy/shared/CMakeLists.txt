add_library(zappy_shared OBJECT
        exception/Exception.cpp
        exception/Exception.hpp
        exception/InvalidAddress.hpp
        exception/InvalidArgument.hpp
        exception/InvalidState.hpp
        exception/SocketError.hpp
        io/IFileDescriptor.hpp
        network/socket/Client.cpp
        network/socket/Client.hpp
        network/socket/Server.cpp
        network/socket/Server.hpp
        network/Address.cpp
        network/Address.hpp
        network/ISocket.hpp
        network/Socket.cpp
        network/Socket.hpp
)
target_include_directories(zappy_shared PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)
