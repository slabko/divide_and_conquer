cmake_minimum_required(VERSION 3.25)
project(merge_sort)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
file(GLOB_RECURSE TEST_FILES test/*.cpp)

add_executable(tests ${SRC_FILES} ${TEST_FILES})
add_executable(main main.cpp ${SRC_FILES})

find_package(Catch2 3 REQUIRED)
find_package(Boost 1.78 REQUIRED)
find_package(fmt 9.1 REQUIRED)

target_link_libraries(tests PUBLIC Catch2::Catch2WithMain fmt::fmt)
target_link_libraries(main PUBLIC fmt::fmt)

include(CTest)
include(Catch)
catch_discover_tests(tests)