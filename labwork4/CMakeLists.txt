cmake_minimum_required(VERSION 3.23)
project(labwork_4_chtonad0000)
set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(labwork_4_chtonad0000
        arch.cpp
        arch.h
        main.cpp
        )
