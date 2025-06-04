cmake_minimum_required(VERSION 3.31)
project(STARREACHER)

set(CMAKE_CXX_STANDARD 20)

add_executable(STARREACHER src/main.cpp
        src/Entity.h
        src/Entity.cpp
        src/Player.h
        src/Player.cpp
        src/Enemy.h
        src/Enemy.cpp
        src/Texture.cpp
        src/Texture.h
        src/Ecran.cpp
        src/Ecran.h)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)

target_link_libraries(STARREACHER
        SDL2::SDL2
        SDL2::SDL2main
        SDL2_image::SDL2_image
        SDL2_mixer::SDL2_mixer)
