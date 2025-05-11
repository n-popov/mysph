cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_FLAGS "-std=c++2a -g")

project(mysph)
add_executable(mysph main.cpp)

# Найдём VTK
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

target_link_libraries(mysph ${VTK_LIBRARIES})
