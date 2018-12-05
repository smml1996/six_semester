cmake_minimum_required(VERSION 3.12)
project(hybridTree)

set(CMAKE_CXX_STANDARD 14)

add_executable(hybridTree main.cpp Node.cpp Node.h HybridTree.cpp HybridTree.h)