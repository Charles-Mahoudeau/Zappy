add_library(zappy_shared OBJECT
        Demo.cpp
        Demo.hpp
)
target_include_directories(zappy_shared PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)
