cmake_minimum_required(VERSION 3.16)
project(unittests)

# This project at least requires C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

if(NOT MSVC)
    # Only used by single-configuration targets (makefile or ninja).
    # Set default build type to Release.
    if(NOT DEFINED CMAKE_BUILD_TYPE OR NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release)
    endif()
endif()
set(CMAKE_DEBUG_POSTFIX d)
if(WIN32)
    add_compile_definitions(UNICODE _UNICODE)
    add_compile_definitions($<$<CONFIG:DEBUG>:_DEBUG>)
    add_compile_definitions($<$<NOT:$<CONFIG:DEBUG>>:NDEBUG>)
    string (REPLACE "/D_WINDOWS" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
endif()
if(MSVC)
    set(CMAKE_GENERATOR_TOOLSET "v142,version=14.29.30133" CACHE INTERNAL "The MSVC toolset version")
    set(CMAKE_CXX_FLAGS_DEBUG "/Od /RTC1")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /GL")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/INCREMENTAL")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/OPT:REF /INCREMENTAL:NO /OPT:ICF")
    add_link_options(/DEBUG)
    add_compile_options(/sdl /FC /GR /Gm- /MP /Zi)
    add_compile_options(/Zc:__cplusplus /permissive- /utf-8 )
endif()
if(UNIX)
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
endif()
if(MSVC)
    add_compile_options(/W4 /wd"4996")
endif()
if(UNIX)
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wswitch-enum
        -Wfloat-conversion
        -Wundef
    )
endif()

add_executable(unittests
    test_main.cpp
    test_access.cpp
    test_ci_string.cpp
    test_fstream.cpp
    test_hello.cpp
    test_refref_param.cpp
    test_stat.cpp
    test_utime.cpp
)
target_link_libraries(unittests gtest gmock)

gtest_discover_tests(unittests
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
)

