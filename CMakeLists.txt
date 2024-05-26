cmake_minimum_required(VERSION 3.14)
project(unittests)

# This project at least requires C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-Wall)
add_definitions(-Wextra)

# reference googletest (git submodule)
add_subdirectory(googletest)
include(GoogleTest)
enable_testing()

mark_as_advanced(
    BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
    gmock_build_tests gtest_build_samples gtest_build_tests
    gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal
    symbols
)

add_executable(test_hello test_hello.cpp)
target_link_libraries(test_hello gtest gmock gtest_main)

gtest_discover_tests(test_hello
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
)

