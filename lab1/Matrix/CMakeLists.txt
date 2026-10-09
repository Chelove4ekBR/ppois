cmake_minimum_required(VERSION 3.31)

project(Matrix)

add_executable(matrix src/main.cpp include/Matrix.h src/Matrix.cpp)

find_package(Catch2 REQUIRED)

add_executable(unit_tests_matr tests/test_matrix.cpp src/Matrix.cpp include/Matrix.h)
target_compile_options(unit_tests_matr PRIVATE --coverage -O0 -g)

target_link_libraries(unit_tests_matr PRIVATE Catch2::Catch2WithMain)
target_link_options(unit_tests_matr PRIVATE --coverage)