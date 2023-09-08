cmake_minimum_required(VERSION 3.22.1)
project(LinkList VERSION 1.0.0.0)

set(LINKEDLIST_SRC
  "${CMAKE_CURRENT_SOURCE_DIR}/src/LinkedList.c"
)

set(LINKEDLIST_HEADERS
  "${CMAKE_CURRENT_SOURCE_DIR}/include/LinkedList.h"
)


set(CMAKE_C_FLAGS "-g")


option(BUILD_TESTS "build tests" ON)
option(BUILD_SHARED_LIBS "building a shared lib" ON)


add_library(LinkedList ${LINKEDLIST_HEADERS} ${LINKEDLIST_SRC})
target_include_directories(LinkedList PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/include")
set_target_properties(LinkedList PROPERTIES C_STANDARD 99)


if(BUILD_TESTS)
    add_subdirectory(test)
endif()

#add_executable(main StructListDemo.c)
#target_link_libraries(main LinkedList)