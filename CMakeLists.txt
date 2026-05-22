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
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        master
    GIT_SHALLOW    TRUE
)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
FetchContent_MakeAvailable(raylib)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} raylib)

if(${PLATFORM} STREQUAL "Web")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
    target_link_options(${PROJECT_NAME} PRIVATE
        "SHELL:--shell-file ${raylib_SOURCE_DIR}/src/minshell.html"
        "SHELL:-s USE_GLFW=3"
        "SHELL:-s TOTAL_MEMORY=67108864"
        "SHELL:-s ALLOW_MEMORY_GROWTH=1"
        "SHELL:-s WASM=1"
    )
endif()
