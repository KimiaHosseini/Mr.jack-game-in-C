cmake_minimum_required(VERSION 3.20)
project(project1 C)

set(CMAKE_C_STANDARD 99)

add_executable(project1 main.c map.h cards.c cards.h action.c action.h round.h round.c map.c game.h game.c)

