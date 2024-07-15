cmake_minimum_required(VERSION 3.23)
project("C:/Users/d_chu/CLionProjects/labwork-10-chtonad0000")

set(CMAKE_CXX_STANDARD 23)
include_directories("C:/Users/d_chu/CLionProjects/PDCurses-3.9")
link_directories("C:/Users/d_chu/CLionProjects/PDCurses-3.9/wincon")
add_executable(labwork_10_chtonad0000
        main.cpp pictures.h Request.cpp Request.h WeatherDrawer.cpp WeatherDrawer.h)

if(WIN32)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()
include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 0817715923c9705e68994eb52ef9df3f6845beba)
FetchContent_MakeAvailable(cpr)
target_link_libraries(labwork_10_chtonad0000 PRIVATE cpr::cpr)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(labwork_10_chtonad0000 PRIVATE nlohmann_json::nlohmann_json)

target_link_libraries(labwork_10_chtonad0000 PRIVATE pdcurses)
set(CMAKE_EXE_LINKER_FLAGS "-static")