cmake_minimum_required(VERSION 3.27)
project(ParallelProgramming)

set(CMAKE_CXX_STANDARD 17)

# added -fopenmp
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

set(SOURCE_FILES main.cpp
        KMeans.cpp
        KMeans.h
        EditDistance.cpp
        EditDistance.h
        Point.cpp
        Point.h
        Cluster.cpp
        Cluster.h
        KMeansOMP.cpp
        KMeansOMP.h
)
add_executable(ParallelProgramming ${SOURCE_FILES})
