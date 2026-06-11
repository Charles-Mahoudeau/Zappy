include(GoogleTest)

set(FIXTURES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/fixtures")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_executable(unit_tests
        src/CliParser.cpp
        src/server/game/EntityDatabase.cpp
        src/shared/network/BufferedClient.cpp
        src/gui/game/GameState.cpp
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
