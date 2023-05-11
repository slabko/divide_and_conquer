cmake_minimum_required(VERSION 3.25)
project(merge_sort)

set(CMAKE_CXX_STANDARD 20)

add_executable(tests tests.cpp merge_sort.cpp product.cpp)
add_executable(main main.cpp product.cpp)

find_package(Catch2 3 REQUIRED)
find_package(Boost 1.78 REQUIRED)
find_package(fmt 9.1 REQUIRED)

target_link_libraries(tests PUBLIC Catch2::Catch2WithMain)
target_link_libraries(main PUBLIC fmt::fmt)

include(CTest)
include(Catch)
catch_discover_tests(tests)