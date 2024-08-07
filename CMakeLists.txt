cmake_minimum_required(VERSION 3.14)
project(OrderCache VERSION 0.1.0 DESCRIPTION "OrderCache library" LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost 1.81.0 REQUIRED COMPONENTS thread system chrono filesystem)
if(Boost_FOUND)
  message(STATUS "Boost found")
  include_directories(${Boost_INCLUDE_DIRS})
else()
  message(FATAL_ERROR "Boost not found")
endif()

add_subdirectory(external/gtest)
add_subdirectory(external/benchmark)
enable_testing()
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(benchmarks)
