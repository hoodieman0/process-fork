cmake_minimum_required(VERSION 3.20)

# ── Fetch GoogleTest ─────────────────────────────────────────────────────────
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)

# Prevent GoogleTest from overriding our compiler / linker settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# ── Test executable ──────────────────────────────────────────────────────────
add_executable(tests
    test_add.cpp
)

target_include_directories(tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(tests PRIVATE
    GTest::gtest_main
)

# ── Register with CTest ──────────────────────────────────────────────────────
include(GoogleTest)
gtest_discover_tests(tests)
