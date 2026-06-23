include(SetupGoogleTest)

set(FIXTURES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/fixtures")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_executable(unit_tests
        src/CliParser.cpp
        src/server/game/EntityDatabase.cpp
        src/server/client/Client.cpp
        src/server/client/ClientRegistry.cpp
        src/server/client/SocketPair.hpp
        src/server/net/SocketRegistry.cpp
        src/server/game/entity/Egg.cpp
        src/server/game/entity/Player.cpp
        src/server/game/entity/Resource.cpp
        src/shared/network/BufferedClient.cpp
        src/gui/game/GameState.cpp
        src/gui/GuiCliParser.cpp
        src/gui/network/CommandSender.cpp
        src/gui/network/Handshake.cpp
        src/gui/network/HandlerRegistry.cpp
        src/gui/network/ProtocolParser.cpp
        src/gui/network/GuiFunctional.cpp
        src/shared/io/Poller.cpp
        src/server/game/World.cpp
        src/shared/math/Vector2.cpp
)
target_include_directories(unit_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(unit_tests PRIVATE
        zappy_shared
        zappy_server_objects
        zappy_gui_objects
        GTest::gtest
        GTest::gmock
        GTest::gmock_main
)
target_compile_definitions(unit_tests
        PRIVATE
        FIXTURES_PATH="${FIXTURES_DIR}"
)

gtest_discover_tests(unit_tests)
