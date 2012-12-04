file(GLOB LIBSSTL_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../include/sstl/_impl/*.cpp)
file(GLOB LIBSSTL_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../include/sstl/[a-z]*[a-z])
list(APPEND LIBSSTL_SOURCE_FILES ${LIBSSTL_HEADER_FILES})
source_group("Header Files" FILES ${LIBSSTL_HEADER_FILES})

if(SSTL_SHARED_LIBRARY)
    add_library(cxx SHARED ${LIBSSTL_SOURCE_FILES})
else()
    add_library(cxx STATIC ${LIBSSTL_SOURCE_FILES})
endif()

add_definitions(-DSSTL_IS_BEING_BUILT)

install(TARGETS cxx
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(DIRECTORY ../include/sstl
        DESTINATION include/sstl
        FILES_MATCHING
        PATTERN "*")
