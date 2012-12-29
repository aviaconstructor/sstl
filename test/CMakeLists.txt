find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

add_subdirectory(compiler)
add_subdirectory(string)
add_subdirectory(types)
