cmake_minimum_required(VERSION 3.23)
project(Microlution)

set(CMAKE_CXX_STANDARD 17)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES OFF CACHE BOOL "" FORCE) # or games

add_subdirectory(include/raylib)

add_executable(${PROJECT_NAME} main.cpp stuff/utils.hpp stuff/logger.hpp microbe.cpp microbe.hpp stuff/intvec.hpp stuff/utils.cpp game.cpp game.hpp entity.hpp grid.cpp grid.hpp entity.cpp stuff/timer.hpp stuff/math.hpp constants.hpp entity_manager.cpp entity_manager.hpp entity_manager.hpp food.cpp food.hpp stuff/global.hpp)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib -static gcc stdc++ winpthread -dynamic)