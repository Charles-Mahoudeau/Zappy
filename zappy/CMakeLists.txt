cmake_minimum_required(VERSION 3.28)
project(zappy)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

add_subdirectory(src/zappy/shared)
add_subdirectory(src/zappy/server)
add_subdirectory(src/zappy/gui)

include(SetupGoogleTest)
setup_googletest()
if (TARGET gtest_main)
    enable_testing()
    add_subdirectory(tests)
endif ()
