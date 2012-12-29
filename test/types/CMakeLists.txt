project(test_types)
cmake_minimum_required(VERSION 2.8)

include_directories(${GTEST_INCLUDE_DIR})

add_executable(test_limits test_limits.cpp)
target_link_libraries(test_limits ${GTEST_BOTH_LIBRARIES})

add_test(NAME test_limits COMMAND test_limits)
