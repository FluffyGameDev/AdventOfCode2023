cmake_minimum_required (VERSION 3.8)

include_directories("${CMAKE_SOURCE_DIR}/external/fmt/include")
include_directories("${CMAKE_SOURCE_DIR}/common")

get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

add_executable (${ProjectId} "solution.cpp")

target_link_libraries(${ProjectId} PRIVATE fmt::fmt-header-only)

add_custom_command(TARGET ${ProjectId} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                           ${CMAKE_CURRENT_SOURCE_DIR}/input.txt
                           ${CMAKE_CURRENT_BINARY_DIR}/input.txt)

set_property(TARGET ${ProjectId} PROPERTY CXX_STANDARD 17)