cmake_minimum_required(VERSION 3.10.0)
project(chip8_emulator
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" ON)

# ---- spdlog (git submodule) ----
# After cloning: git submodule update --init --recursive
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/submodules/spdlog/CMakeLists.txt")
    message(FATAL_ERROR "spdlog submodule missing. Run: git submodule update --init --recursive")
endif()
add_subdirectory(submodules/spdlog)

# Logger wrapper (utils/logger.hpp) as one target to link against.
# Compile-time floor: LOG_TRACE/LOG_DEBUG are compiled out in Release builds.
add_library(chip8_logger INTERFACE)
target_include_directories(chip8_logger INTERFACE "${CMAKE_SOURCE_DIR}/utils")
target_link_libraries(chip8_logger INTERFACE spdlog::spdlog)
target_compile_definitions(chip8_logger INTERFACE
    $<IF:$<CONFIG:Release>,SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO,SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE>
)

# ---- Emulator core library ----
# Everything in src/ goes here; main and the unit tests both link to it,
# so whatever you write in the library is testable straight away.
add_library(chip8 STATIC src/chip8.cpp)
target_include_directories(chip8 PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(chip8 PUBLIC chip8_logger)

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
    enable_testing()  # lets `ctest` run from the build root
    add_subdirectory(tests)
endif(BUILD_TESTS)

add_subdirectory(playground)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE chip8)

# Use of install to ensure system wide proper installation
# https://stackoverflow.com/a/74313654
install(TARGETS)
