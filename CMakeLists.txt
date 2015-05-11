cmake_minimum_required(VERSION 3.0)

project(hanal)
set(hanal_VERSION_MAJOR 0)
set(hanal_VERSION_MINOR 1)
set(hanal_VERSION_PATCH 0)
set(hanal_VERSION ${hanal_VERSION_MAJOR}.${hanal_VERSION_MINOR}.${hanal_VERSION_PATCH})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(src/main/cpp)
aux_source_directory(src/main/cpp/hanal src_main_cpp_hanal)

add_library(hanal SHARED ${src_main_cpp_hanal})
set_target_properties(hanal PROPERTIES VERSION ${hanal_VERSION} SOVERSION ${hanal_VERSION_MAJOR})
