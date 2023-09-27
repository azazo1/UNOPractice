cmake_minimum_required(VERSION 3.26)
project(UNO C)

set(CMAKE_C_STANDARD 11)

add_executable(UNO src/main.c
        src/card.h
        src/game.h
        src/game.c
        src/card.c
        src/player.c
        src/player.h)
