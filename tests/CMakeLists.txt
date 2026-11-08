# Unit tests. Add a new test file to the list below and it gets built + discovered.
add_executable(test_main
  unit/main.cpp
  unit/test_instructions.cpp
)

target_link_libraries(test_main
  GTest::gtest_main
  chip8
)

include(GoogleTest)
gtest_discover_tests(test_main)
