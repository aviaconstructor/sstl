project(libsstl)
cmake_minimum_required(VERSION 2.8)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" _SOURCE_AND_BINARY_DIRS_ARE_THE_SAME)
if(_SOURCE_AND_BINARY_DIRS_ARE_THE_SAME)
    message(FATAL_ERROR "Please build out of source:
                         1. Creating an empty directory
                         2. Step there and run the command: `cmake path/to/sstl/root`, supply options if desired
                         3. Make the make file created")
endif()

option(SSTL_SHARED_LIBRARY "Create shared library (so or dll), otherwise statically linked" OFF)

set(PACKAGE_NAME ${CMAKE_PROJECT_NAME})
set(PACKAGE_VERSION 0.1)
set(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")

include_directories(include)
add_subdirectory(lib/sstl)
add_subdirectory(test)
