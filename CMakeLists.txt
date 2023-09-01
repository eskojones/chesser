cmake_minimum_required(VERSION 3.12)
project(Chesser)

set(SDL2_INCLUDE_PATH /opt/homebrew/Cellar/sdl2/2.28.2/include)
set(SDL2_LIBRARY_PATH /opt/homebrew/Cellar/sdl2/2.28.2/lib)
set(CMAKE_C_FLAGS -Wno-void-pointer-to-int-cast)
include("${SDL2_LIBRARY_PATH}/cmake/SDL2/sdl2-config.cmake")
add_executable(
    chesser
    main.c
    chess.c
    list.c
    font.c
    font_5x5.c
    font_7x7.c
    surface.c
)

add_library(SDL2main STATIC IMPORTED)
add_library(SDL2 STATIC IMPORTED)
set_target_properties(
    SDL2main
    PROPERTIES
    IMPORTED_LOCATION "${SDL2_LIBRARY_PATH}/libSDL2main.a"
    INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_PATH}"
)

set_target_properties(
    SDL2
    PROPERTIES
    IMPORTED_LOCATION "${SDL2_LIBRARY_PATH}/libSDL2.a"
    INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_PATH}"
)

target_link_libraries(chesser SDL2 SDL2main)
