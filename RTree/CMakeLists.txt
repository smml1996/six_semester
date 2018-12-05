cmake_minimum_required(VERSION 3.12)
project(RTree)

set(CMAKE_CXX_STANDARD 14)

add_executable(RTree main.cpp Node.cpp Node.h util.cpp RTree.cpp RTree.h)