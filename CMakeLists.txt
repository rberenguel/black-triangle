cmake_minimum_required(VERSION 3.15)
project(black_triangle CXX)

set(CMAKE_CXX_STANDARD 17)

# Must be in cache before FetchContent so raylib picks it up during its configure
if(NOT DEFINED PLATFORM)
    set(PLATFORM "Desktop" CACHE STRING "Raylib platform")
else()
    set(PLATFORM "${PLATFORM}" CACHE STRING "Raylib platform" FORCE)
endif()

include(FetchContent)

# ── raylib ────────────────────────────────────────────────────────────────────
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        master
    GIT_SHALLOW    TRUE
)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
FetchContent_MakeAvailable(raylib)

# ── game executable ───────────────────────────────────────────────────────────
add_executable(${PROJECT_NAME} src/main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} raylib)

if(CMAKE_CROSSCOMPILING AND NOT EMSCRIPTEN)
    target_compile_definitions(${PROJECT_NAME} PRIVATE SCREEN_WIDTH=480 SCREEN_HEIGHT=640 SCREEN_ROTATE=1)
endif()

if(${PLATFORM} STREQUAL "Web")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html" OUTPUT_NAME "index")
    target_link_options(${PROJECT_NAME} PRIVATE
        "SHELL:--shell-file ${CMAKE_SOURCE_DIR}/src/shell.html"
        "SHELL:-s USE_GLFW=3"
        "SHELL:-s TOTAL_MEMORY=67108864"
        "SHELL:-s ALLOW_MEMORY_GROWTH=1"
        "SHELL:-s WASM=1"
    )
endif()

# ── tests (Desktop / Mac only) ────────────────────────────────────────────────
if(NOT CMAKE_CROSSCOMPILING AND NOT ${PLATFORM} STREQUAL "Web")
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    aux_source_directory(tests TEST_SOURCES)
    add_executable(tests ${TEST_SOURCES})
    target_include_directories(tests PRIVATE src)
    target_link_libraries(tests GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(tests)
endif()
