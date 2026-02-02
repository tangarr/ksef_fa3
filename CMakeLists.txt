cmake_minimum_required(VERSION 3.19)
project(ksef LANGUAGES CXX)

find_package(Qt6 6.10 REQUIRED COMPONENTS Core Widgets)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.17.0
)
FetchContent_MakeAvailable(googletest)

set(CMAKE_CXX_STANDARD 23)

add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Wnrvo -Werror=switch)

# qt_standard_project_setup()

# qt_add_executable(ksef
#     WIN32 MACOSX_BUNDLE
#     main.cpp
#     #mainwindow.ui
# )

# target_link_libraries(ksef
#     PRIVATE
#         Qt::Core
#         #Qt::Widgets
# )

include(GNUInstallDirs)

# install(TARGETS ksef
#     BUNDLE  DESTINATION .
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
# )

# qt_generate_deploy_app_script(
#     TARGET ksef
#     OUTPUT_SCRIPT deploy_script
#     NO_UNSUPPORTED_PLATFORM_ERROR
# )
# install(SCRIPT ${deploy_script})

option(COVERAGE "Enable code coverage" OFF)

enable_testing()

add_subdirectory(lib)
add_subdirectory(tests)

