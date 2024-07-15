cmake_minimum_required(VERSION 3.20)
project(labwork_9_chtonad0000)

set(CMAKE_CXX_STANDARD 23)


add_executable(labwork_9_chtonad0000
        memory_pool.h
        MemoryPoolAllocator.h
        main.cpp memory_pool.cpp)
set(CMAKE_EXE_LINKER_FLAGS "-static")
