cmake_minimum_required(VERSION 3.23)
project(labwork_12_chtonad0000)

set(CMAKE_CXX_STANDARD 23)

add_executable(labwork_12_chtonad0000
        main.cpp MyDB.cpp MyDB.h)
add_subdirectory(tests)