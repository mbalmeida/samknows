cmake_minimum_required(VERSION 3.8)
project(samknows)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c utils/Heap.h utils/Heap.c HTTPStats.h)
add_executable(samknows ${SOURCE_FILES})

add_library(HTTPStats SHARED HTTPStats.c)

add_library(HTTPStatsCurlImpl SHARED HTTPStatsCurlImpl.c)

target_link_libraries(HTTPStats HTTPStatsCurlImpl)

target_link_libraries(HTTPStatsCurlImpl curl)

target_link_libraries(samknows HTTPStats HTTPStatsCurlImpl)