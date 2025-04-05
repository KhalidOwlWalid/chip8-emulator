cmake_minimum_required(VERSION 3.10.0)
project(chip8_emulator
    VERSION 0.1.0
    LANGUAGES CXX
)

option(BUILD_TESTS "Build tests" ON)

if(BUILD_TESTS)
    execute_process(
        # Ping twice with a timeout of 5 seconds
        # This command works for linux, for windows, please double check if it is compatible or not
        COMMAND ping www.google.com -c 2 -w 5
        ERROR_QUIET
        RESULT_VARIABLE NO_CONNECTION
    )

    if (NOT NO_CONNECTION EQUAL 0)
        set(BUILD_TESTS OFF)
        message(WARNING "Device is not connected to the internet! Test builds will not proceed.")
    endif()
endif(BUILD_TESTS)

if(BUILD_TESTS)
    message(STATUS "Building unittest for ${PROJECT_NAME}")
    # Fetch content actually requires internet connection
    # However, this only needs to be done once you have make before
    include(FetchContent)
    FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.16.0.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    add_subdirectory(tests)
endif(BUILD_TESTS)

add_executable(main main.cpp)

target_include_directories(main PUBLIC
    "${CMAKE_SOURCE_DIR}/utils/logger.hpp"
)

# Use of install to ensure system wide proper installation
# https://stackoverflow.com/a/74313654
install(TARGETS)
