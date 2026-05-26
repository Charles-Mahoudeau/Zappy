macro(check_internet_connection)
    set(INTERNET_AVAILABLE FALSE)

    file(DOWNLOAD "https://api.github.com/zen"
            "${CMAKE_CURRENT_BINARY_DIR}/.github.ping.txt"
            TIMEOUT 5
            STATUS ping_status
    )
    list(GET ping_status 0 ping_error_code)

    if (ping_error_code EQUAL 0)
        set(INTERNET_AVAILABLE TRUE)
    endif ()
endmacro()
