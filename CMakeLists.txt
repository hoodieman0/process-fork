cmake_minimum_required(VERSION 3.20)

project(
    process-fork
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

add_executable(messagebroker
    src/messagebroker/main.cpp
    src/messagebroker/Broker.cpp
)

target_include_directories(messagebroker PRIVATE
    ${CMAKE_SOURCE_DIR}/src/messagebroker
)

add_executable(logger
    src/logger/main.cpp
)

add_executable(producer
    src/producer/main.cpp
)

add_executable(processor
    src/processor/main.cpp
)

add_executable(controller
    src/controller/main.cpp
)



# ── Testing ──────────────────────────────────────────────────────────────────
include(CTest)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
