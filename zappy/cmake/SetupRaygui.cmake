include(FetchContent)
include(InternetCheck)

check_internet_connection()

macro(setup_raygui)
    if (INTERNET_AVAILABLE)
        FetchContent_Declare(
                raygui
                GIT_REPOSITORY https://github.com/raysan5/raygui.git
                GIT_TAG 4.0
        )

        FetchContent_MakeAvailable(raygui)
    else ()
        message(WARNING "raygui not available: no internet connection.")
    endif ()
endmacro()
