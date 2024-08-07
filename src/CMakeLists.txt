set(LIB_SOURCE_FILES
  OrderCacheSimple.cpp
  OrderCacheMap.cpp
  OrderCacheMapThread.cpp
  CompanyQuantity.cpp
  TaskQueue.cpp
  utils.cpp
)

add_library(
  ${PROJECT_NAME}
  SHARED
  ${LIB_SOURCE_FILES}
)

set(ORDER_CACHE_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC
  ${Boost_LIBRARIES}
)

target_include_directories(
  ${PROJECT_NAME}
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${ORDER_CACHE_INCLUDE_DIRS}
)
set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES
  VERSION ${PROJECT_VERSION}
)

set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES
  PUBLIC_HEADER OrderCache.h
)


install(
  TARGETS OrderCache
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
