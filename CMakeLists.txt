cmake_minimum_required(VERSION 3.28)
project(StatisticalSerenade)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

add_executable(StatisticalSerenade main.cpp
        NaryTree.cpp
        NaryTree.h)

target_link_libraries(StatisticalSerenade PRIVATE nlohmann_json::nlohmann_json)
