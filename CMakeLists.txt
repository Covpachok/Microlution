cmake_minimum_required(VERSION 3.23)
project(Microlution)

set(CMAKE_CXX_STANDARD 17)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES OFF CACHE BOOL "" FORCE) # or games

add_subdirectory(include/raylib)

add_executable(${PROJECT_NAME} main.cpp utils.hpp logger.hpp microbe.cpp microbe.hpp intvec.hpp utils.cpp game.cpp game.hpp entity.hpp vegetable.cpp vegetable.hpp grid.cpp grid.hpp entity.cpp timer.hpp math.hpp constants.hpp)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib -static gcc stdc++ winpthread -dynamic)