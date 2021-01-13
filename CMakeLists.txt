cmake_minimum_required(VERSION 3.17)
project(SpaceInvaders)
include(FindPkgConfig)

set(CMAKE_CXX_STANDARD 17)

# including SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(SpaceInvaders main.cpp constants.h Entities/Ship.h Entities/Player.cpp Entities/Player.h Entities/Enemy.cpp Entities/Enemy.h Entities/Bullet.cpp Entities/Bullet.h)
target_link_libraries(SpaceInvaders ${SDL2_LIBRARIES})