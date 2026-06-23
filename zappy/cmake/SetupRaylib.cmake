include(FetchContent)

FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 6.0
        FIND_PACKAGE_ARGS NAMES raylib CONFIG
)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DSUPPORT_GPU_SKINNING=1" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSUPPORT_GPU_SKINNING=1" CACHE STRING "" FORCE)

FetchContent_MakeAvailable(raylib)
