cmake_minimum_required(VERSION 3.20)

project(main)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-Wall")

include_directories(${CMAKE_SOURCE_DIR})
link_directories(${CMAKE_SOURCE_DIR})

add_executable(main main.cpp)
target_link_libraries(main PRIVATE lib)
