include_directories(${ORDER_CACHE_INCLUDE_DIRS})
set(BENCHMARK_SOURCE_DIR 
  ${CMAKE_SOURCE_DIR}/benchmark
)
include_directories(${CMAKE_SOURCE_DIR})
set(BENCHMAKR_ENABLE_GTEST_TESTS OFF)


# Files common to all tests
set (BENCHMARK_COMMON_SOURCES
  base.cpp
  companyQauntity.cpp
  main.cpp
)

set(SETUP_BENCHMARK_NAME benchmark_order_cache)


add_executable(${SETUP_BENCHMARK_NAME} ${BENCHMARK_COMMON_SOURCES})
target_link_libraries(${SETUP_BENCHMARK_NAME} ${PROJECT_NAME}   benchmark::benchmark)
target_compile_definitions(${SETUP_BENCHMARK_NAME} PRIVATE SOURCE_DIR="${CMAKE_SOURCE_DIR}")
add_test(${SETUP_BENCHMARK_NAME} ${SETUP_BENCHMARK_NAME})
