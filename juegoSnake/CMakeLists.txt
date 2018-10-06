cmake_minimum_required(VERSION 3.12)
project(juegoSnake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-lncurses")

add_executable(juegoSnake main.cpp Snake.cpp Snake.h)