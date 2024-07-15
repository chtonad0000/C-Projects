cmake_minimum_required(VERSION 3.23)
project(labwork1)

set(CMAKE_CXX_STANDARD 14)

add_executable(labwork1 main.cpp)
set(CMAKE_EXE_LINKER_FLAGS "-static")