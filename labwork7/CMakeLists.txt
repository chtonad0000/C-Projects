cmake_minimum_required(VERSION 3.23)
project(labwork_7_chtonad0000)

set(CMAKE_CXX_STANDARD 23)

add_executable(labwork_7_chtonad0000 main.cpp
        parser_ID3v2.h
        parser_ID3v2.cpp
        frames_printer.cpp
        frames_reader.cpp
        library.h)
