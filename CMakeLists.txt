cmake_minimum_required(VERSION 2.8)

project(hanal)
set(hanal_VERSION_MAJOR 0)
set(hanal_VERSION_MINOR 1)
set(hanal_VERSION_PATCH 0)
set(hanal_VERSION ${hanal_VERSION_MAJOR}.${hanal_VERSION_MINOR}.${hanal_VERSION_PATCH})

option(GCOV "on/off gcov options" OFF)

find_package(Boost COMPONENTS iostreams log)
find_package(Threads)

if (NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  set(CMAKE_BUILD_TYPE "Debug")
endif()
if (GCOV)
  message(STATUS "gcov option enabled")
  set(CMAKE_BUILD_TYPE "Debug")
  add_definitions(
    -fprofile-arcs
    -ftest-coverage
  )
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -lgcov -coverage")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgcov -coverage")
endif()
include_directories(
  include
  src/main/cpp
  src/test/cpp
  ${Boost_INCLUDE_DIR}
)
add_definitions(
  -std=c++11
  -DBOOST_LOG_DYN_LINK
)
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  add_definitions(-g3)
endif()
aux_source_directory(src/main/cpp/hanal src_main_cpp_hanal)

add_library(hanal SHARED ${src_main_cpp_hanal})
set_target_properties(hanal PROPERTIES VERSION ${hanal_VERSION} SOVERSION ${hanal_VERSION_MAJOR})
target_link_libraries(hanal ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

aux_source_directory(src/test/cpp/hanal src_test_cpp_hanal)
aux_source_directory(src/test/cpp src_test_cpp)
add_executable(test_hanal ${src_test_cpp_hanal} ${src_test_cpp})
target_link_libraries(test_hanal hanal ${Boost_LIBRARIES})

enable_testing()
add_test(test_hanal test_hanal "--rsc-dir=${CMAKE_SOURCE_DIR}/rsc")
