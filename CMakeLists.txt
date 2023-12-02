cmake_minimum_required(VERSION 3.24)
project(PROJECT3_GRAPH)

set(CMAKE_CXX_STANDARD 14)

# Find the necessary packages
find_package(CURL REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

add_executable(PROJECT3_GRAPH main.cpp)

# Link libraries
target_link_libraries(PROJECT3_GRAPH PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)
