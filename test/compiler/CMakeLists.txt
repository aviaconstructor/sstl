project(test_compiler)
cmake_minimum_required(VERSION 2.8)

add_executable(test_malloc_new test_malloc_new.cpp)
target_link_libraries(test_malloc_new ${GTEST_BOTH_LIBRARIES})

add_test(NAME test_malloc_new COMMAND test_malloc_new)
