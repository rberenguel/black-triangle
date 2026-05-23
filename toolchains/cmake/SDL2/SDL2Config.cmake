set(SDL2_INCLUDE_DIRS "/usr/include/SDL2")
set(SDL2_LIBRARIES "/src/toolchains/libs/libSDL2-2.0.so.0")
set(SDL2_FOUND TRUE)
set(SDL2_VERSION "2.0.0")

if(NOT TARGET SDL2::SDL2)
    add_library(SDL2::SDL2 SHARED IMPORTED)
    set_target_properties(SDL2::SDL2 PROPERTIES
        IMPORTED_LOCATION "/src/toolchains/libs/libSDL2-2.0.so.0"
        INTERFACE_INCLUDE_DIRECTORIES "/usr/include/SDL2"
    )
endif()

if(NOT TARGET SDL2::SDL2main)
    add_library(SDL2::SDL2main INTERFACE IMPORTED)
endif()
