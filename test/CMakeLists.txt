cmake_minimum_required(VERSION 3.22.1)

find_library(CMOCKA_LIBRARIES REQUIRED NAMES cmocka)
if(CMOCKA_LIBRARIES)
    message(STATUS "cmocka library found: ${CMOCKA_LIBRARIES}")
else()
    #cmocka not found
    message(FATAL_ERROR "cmocka library not found please install cmocka to run these tests")
endif()

set(TEST_LINKEDLIST "${CMAKE_CURRENT_SOURCE_DIR}/LinkedListTests.c")

add_executable(LinkedList_test ${TEST_LINKEDLIST})

set_target_properties(LinkedList_test PROPERTIES C_STANDARD 99)

target_link_libraries(LinkedList_test PRIVATE LinkedList)
target_link_libraries(LinkedList_test PRIVATE cmocka)