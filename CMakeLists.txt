cmake_minimum_required(VERSION 3.0)

project(hanal)
set(hanal_VERSION_MAJOR 0)
set(hanal_VERSION_MINOR 1)
set(hanal_VERSION_PATCH 0)
set(hanal_VERSION ${hanal_VERSION_MAJOR}.${hanal_VERSION_MINOR}.${hanal_VERSION_PATCH})

find_package(Boost COMPONENTS iostreams)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(
  include
  src/main/cpp
  ${Boost_INCLUDE_DIR}
)
aux_source_directory(src/main/cpp/hanal src_main_cpp_hanal)

add_library(hanal SHARED ${src_main_cpp_hanal})
set_target_properties(hanal PROPERTIES VERSION ${hanal_VERSION} SOVERSION ${hanal_VERSION_MAJOR})
target_link_libraries(hanal ${Boost_LIBRARIES})
