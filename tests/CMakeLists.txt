cmake_minimum_required(VERSION 3.14)
project(test_project)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_executable(test_main
  unit/main.cpp
)

# Add include directories
# target_include_directories(test_main PRIVATE
#   # "/home/khalidowlwalid/Documents/KhalidOWlWalid-Github-Projects/chip8-emulator/tests/googletest/googletest/include"
#   # "${CMAKE_SOURCE_DIR}/libraries/googletest/googletest/include"
# )

# Link against the pre-built libraries
target_link_libraries(test_main
  # "/home/khalidowlwalid/Documents/KhalidOWlWalid-Github-Projects/chip8-emulator/tests/googletest/build/lib/libgtest.a"
  # "/home/khalidowlwalid/Documents/KhalidOWlWalid-Github-Projects/chip8-emulator/tests/googletest/build/lib/libgtest_main.a"
  # "${CMAKE_SOURCE_DIR}/build/lib/libgtest.a"
  # "${CMAKE_SOURCE_DIR}/build/lib/libgtest_main.a"
  GTest::gtest_main
  pthread  # gtest often requires pthread
)

include(GoogleTest)
gtest_discover_tests(test_main)

