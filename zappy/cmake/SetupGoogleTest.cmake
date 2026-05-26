include(FetchContent)
include(InternetCheck)

check_internet_connection()

macro(setup_googletest)
    if (INTERNET_AVAILABLE)
        FetchContent_Declare(
                googletest
                GIT_REPOSITORY https://github.com/google/googletest.git
                GIT_TAG v1.17.0
        )

        set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

        FetchContent_MakeAvailable(googletest)
    else ()
        message(WARNING "GoogleTest not available.")
    endif ()
endmacro()
