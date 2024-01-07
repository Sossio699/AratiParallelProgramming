cmake_minimum_required(VERSION 3.27)
project(ParallelProgramming)

set(CMAKE_CXX_STANDARD 17)

# added -fopenmp
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

set(SOURCE_FILES main.cpp)
add_executable(ParallelProgramming ${SOURCE_FILES})
