cmake_minimum_required(VERSION 3.12)
project(edaRTree)

set(CMAKE_CXX_STANDARD 14)

add_executable(edaRTree main.cpp RTree.cpp RTree.h Node.cpp Node.h util.cpp)