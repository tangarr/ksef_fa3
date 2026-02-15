cmake_minimum_required(VERSION 3.19)

project(ksef_fa3 LANGUAGES CXX)
project(${PROJECT_NAME} VERSION 0.9.0)

find_package(Qt6 6.10 REQUIRED COMPONENTS Core)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.17.0
)
FetchContent_MakeAvailable(googletest)

set(CMAKE_CXX_STANDARD 23)

add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Wnrvo -Werror=switch)

include(GNUInstallDirs)

option(COVERAGE "Enable code coverage" OFF)

enable_testing()

add_subdirectory(lib)
add_subdirectory(tests)

