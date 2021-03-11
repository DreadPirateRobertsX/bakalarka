cmake_minimum_required(VERSION 3.17)
project(ForensX)

set(CMAKE_CXX_STANDARD 17)

add_executable(ForensX main.cpp Processes.cpp MyProcesses.h)