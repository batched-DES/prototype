cmake_minimum_required(VERSION 3.9)
project(batched_DES)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-unused-variable -march=native -O3 -ftemplate-depth=10000000")

include_directories(batchscheduler model)

add_executable(batched_DES batchsimulator.cpp batchscheduler/BatchScheduler.cpp)