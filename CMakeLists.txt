cmake_minimum_required(VERSION 3.12)
project(Chesser)

add_executable(
    chesser
    main.c
    chess.c
)

target_link_libraries(chesser)
