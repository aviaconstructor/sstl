project(libsstl)
cmake_minimum_required(VERSION 2.8)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" _SOURCE_AND_BINARY_DIRS_ARE_THE_SAME)
if(_SOURCE_AND_BINARY_DIRS_ARE_THE_SAME)
    message(FATAL_ERROR "Please build out of source:
                         1. Creating an empty directory
                         2. Step there and run the command: `cmake path/to/sstl/root`, supply options if desired
                         3. Make the make file created")
endif()

option(SSTL_SHARED_LIBRARY  "Create shared library (so or dll), otherwise statically linked" OFF)
option(SSTL_TEST_SSTL       "Build SSTL tests" ON)
option(SSTL_TEST_NATIVE_STL "Test environment's native STL with SSTL tests" ON)

set(PACKAGE_NAME ${CMAKE_PROJECT_NAME})
set(PACKAGE_VERSION 0.1)
set(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")

enable_testing()

include_directories(include)
add_subdirectory(lib/sstl)
if(SSTL_TEST_SSTL OR SSTL_TEST_NATIVE_STL)
   add_subdirectory(test)
endif()
