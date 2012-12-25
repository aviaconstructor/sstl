project(test_string)
cmake_minimum_required(VERSION 2.8)

include_directories(${GTEST_INCLUDE_DIR})

add_executable(test_string test_string.cpp)
target_link_libraries(test_string ${GTEST_BOTH_LIBRARIES})

add_test(NAME test_string COMMAND test_string)
