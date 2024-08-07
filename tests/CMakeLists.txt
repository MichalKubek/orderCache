include_directories(${ORDER_CACHE_INCLUDE_DIRS})
set(TEST_SOURCE_DIR 
  ${CMAKE_SOURCE_DIR}/tests/test_order_cache.cpp
)
include_directories(${CMAKE_SOURCE_DIR})

# Variable from here is empty
message(STATUS "Include Directories: ${ORDER_CACHE_INCLUDE_DIRS}")

# Files common to all tests
set (TEST_COMMON_SOURCES
  test_orderCache.cpp
  test_companyQuantity.cpp
    )

# Library setup/shutdown testing
set(SETUP_TEST_NAME test_order_cache)

add_executable(${SETUP_TEST_NAME} ${TEST_COMMON_SOURCES})
target_link_libraries(${SETUP_TEST_NAME} ${PROJECT_NAME}   GTest::gtest_main OrderCache ${Boost_LIBRARIES})
add_test(${SETUP_TEST_NAME} ${SETUP_TEST_NAME})
