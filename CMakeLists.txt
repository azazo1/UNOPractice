cmake_minimum_required(VERSION 3.26)
project(UNO C)

set(CMAKE_C_STANDARD 11)

add_executable(UNO src/main.c
        src/card.h
        src/game.h
        src/game.c
        src/card.c
        src/player.c
        src/player.h
        src/toclient.c
        src/toclient.h
        src/msg.h
        src/server.c
        src/server.h
        src/msg.c)
target_link_libraries(UNO wsock32 ws2_32) # 我的妈终于好了 https://stackoverflow.com/questions/15119639/how-to-link-winsock-in-cmake