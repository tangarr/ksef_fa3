add_executable(run_tests
  test_main.cpp
  test_examples.cpp
  helpers.hpp
)
target_link_libraries(run_tests
  gtest_main
  gtest
  ksef_fa3
  Qt6::Core
)
target_compile_definitions(run_tests PRIVATE SRCDIR="${CMAKE_CURRENT_SOURCE_DIR}")
include(GoogleTest)
gtest_discover_tests(run_tests DISCOVERY_MODE PRE_TEST)

if(COVERAGE)
    target_compile_options(run_tests PRIVATE --coverage)
    target_link_options(run_tests PRIVATE --coverage)
endif()
