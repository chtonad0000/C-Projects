cmake_minimum_required(VERSION 3.20)
project(labwork_8_chtonad0000)

set(CMAKE_CXX_STANDARD 23)


add_executable(labwork_8_chtonad0000
        CCirtucalBufferExt.h
        Iterator.h
        CCirtucalBuffer.h
        main.cpp)
add_subdirectory(tests)