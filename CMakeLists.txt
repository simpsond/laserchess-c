cmake_minimum_required(VERSION 3.17)
project(laser_chess_c C)

set(CMAKE_C_STANDARD 99)

#list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)
#set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
#INCLUDE(CodeCoverage)

#find_library(SDL2_LIBRARY SDL2)
find_package(SDL2 REQUIRED)
#find_package(SDL2_gfx REQUIRED)
find_package(PkgConfig)
pkg_check_modules(SDL2_gfx SDL2_gfx)
message("sdl: ${SDL2_INCLUDE_DIRS}")
message("sdl: ${SDL2_LIBRARY}")

message("sdl_gfx: ${SDL2_gfx_INCLUDE_DIRS}")
message("sdl_gfx: ${SDL2_gfx_LIBRARIES}")

#find_package(LibCBOR REQUIRED)

include_directories(/usr/local/include ${SDL2_INCLUDE_DIRS} ${SDL2_gfx_INCLUDE_DIRS})
link_directories(/usr/local/lib ${SDL2_LIBRARY_DIRS} ${SDL2_gfx_LIBRARY_DIRS})
#link_directories(/usr/local/lib)

add_executable(laser_chess_c main.c laser-chess.c utils.c drawing.c laser-beam.c GameState.c Piece.c)
target_link_libraries(laser_chess_c ${SDL2_gfx_LIBRARIES} m cbor)

add_executable(list-test tests/listtest.c linked-list.c)

add_executable(test_utils tests/test_utils.c utils.c)
target_link_libraries(test_utils ${SDL2_gfx_LIBRARIES} cmocka m)
add_test(test_utils test_utils)

#SET(
#        COVERAGE_EXCLUDES
#        include/
#)
#
#SETUP_TARGET_FOR_COVERAGE(
#        NAME test_coverage
#        EXECUTABLE ctest
#        DEPENDENCIES mylib
#)