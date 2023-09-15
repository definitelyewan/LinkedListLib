cmake_minimum_required(VERSION 3.22.1)
project(LinkList VERSION 1.0.0.0)

set(LINKEDLIST_SRC
  "${CMAKE_CURRENT_SOURCE_DIR}/src/LinkedList.c"
)

set(LINKEDLIST_HEADERS
  "${CMAKE_CURRENT_SOURCE_DIR}/include/LinkedList.h"
)

option(BUILD_TESTS "build tests" OFF)
option(BUILD_DOCS "create documentation" OFF)
option(BUILD_DEBUG "debug symbols" OFF)
option(BUILD_SHARED_LIBS "building a shared lib" ON)


if(BUILD_DEBUG)
    set(CMAKE_C_FLAGS "-g")
endif()

add_library(LinkedList ${LINKEDLIST_HEADERS} ${LINKEDLIST_SRC})
target_include_directories(LinkedList PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/include")
set_target_properties(LinkedList PROPERTIES C_STANDARD 99)


if(BUILD_TESTS)
    add_subdirectory(test)
endif()

if(BUILD_DOCS)
    add_subdirectory(doc)
endif()




if(UNIX AND NOT APPLE)
  include(GNUInstallDirs)
  set(CMAKE_INSTALL_PREFIX "/usr")
  install(TARGETS LinkedList DESTINATION lib)
  install(FILES ${LINKEDLIST_HEADERS} DESTINATION include)
  
elseif(WIN32)
  set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
  set(CPACK_PACKAGE_INSTALL_DIRECTORY "LinkedListLib")
  set(CPACK_GENERATOR "NSIS")
  set(CPACK_NSIS_PACKAGE_NAME "LinkedListLib")
  install(TARGETS LinkedList DESTINATION lib)
  install(FILES ${LINKEDLIST_HEADERS} DESTINATION include)
  set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
  set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
  set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
  include(CPack)
elseif(APPLE)
#i dont have mac hardware :(
endif()

include(InstallRequiredSystemLibraries)