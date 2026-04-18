cmake_minimum_required(VERSION 3.20)

project(
    MyProject
    VERSION 0.1.0
    LANGUAGES CXX
)

# ── Language standard ────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)          # Keeps us portable across compilers

# ── Export compile commands (useful for clangd / VS Code IntelliSense) ───────
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ── Compiler-agnostic warning flags ─────────────────────────────────────────
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Werror -Wall -Wextra -Wpedantic)
endif()

# ── Main executable ──────────────────────────────────────────────────────────
add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# ── Testing ──────────────────────────────────────────────────────────────────
include(CTest)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
