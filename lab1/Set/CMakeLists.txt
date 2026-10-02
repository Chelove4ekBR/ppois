cmake_minimum_required(VERSION 3.31)

project(Set)

add_executable(set src/main.cpp src/Set.h src/Set.cpp)

find_package(Catch2 REQUIRED)

add_executable(unit_tests_set tests/test_set.cpp src/Set.cpp src/Set.h)
target_compile_options(unit_tests_set PRIVATE --coverage -O0 -g)

target_link_libraries(unit_tests_set PRIVATE Catch2::Catch2WithMain)
target_link_options(unit_tests_set PRIVATE --coverage)
