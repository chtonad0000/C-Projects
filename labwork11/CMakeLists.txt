cmake_minimum_required(VERSION 3.23)
project(labwork_11_chtonad0000)

set(CMAKE_CXX_STANDARD 23)


add_executable(labwork_11_chtonad0000
        algorithms.h
        xrange.h
        zip.h
        main.cpp)
add_subdirectory(tests)