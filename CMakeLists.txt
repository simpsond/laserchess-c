cmake_minimum_required(VERSION 3.17)
project(laser_chess_c C)

set(CMAKE_C_STANDARD 99)

#find_library(SDL2_LIBRARY SDL2)
find_package(SDL2 REQUIRED)
find_package(PkgConfig)
pkg_check_modules(SDL2_gfx SDL2_gfx)
message("sdl: ${SDL2_INCLUDE_DIRS}")
message("sdl: ${SDL2_LIBRARY}")

message("sdl_gfx: ${SDL2_gfx_INCLUDE_DIRS}")
message("sdl_gfx: ${SDL2_gfx_LIBRARIES}")

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_gfx_INCLUDE_DIRS})
link_directories(${SDL2_LIBRARY_DIRS} ${SDL2_gfx_LIBRARY_DIRS})
#link_directories(/usr/local/lib)

add_executable(laser_chess_c main.c laser-chess.c utils.c drawing.c laser-beam.c)
target_link_libraries(laser_chess_c ${SDL2_gfx_LIBRARIES})

add_executable(list-test tests/listtest.c linked-list.c)