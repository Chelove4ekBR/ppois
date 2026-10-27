cmake_minimum_required(VERSION 3.16)
project(ComputerSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB COMPUTER_SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_library(computer_lib ${COMPUTER_SOURCES})
target_include_directories(computer_lib PUBLIC include)
target_compile_options(computer_lib PRIVATE -Wall -Wextra -Wpedantic)

add_executable(computer_cli cli/main.cpp)
target_link_libraries(computer_cli PRIVATE computer_lib)
target_compile_options(computer_cli PRIVATE -Wall -Wextra -Wpedantic)

find_package(Catch2 REQUIRED)
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS tests/*.cpp)

add_library(computer_lib_cov STATIC ${COMPUTER_SOURCES})
target_include_directories(computer_lib_cov PUBLIC include)
target_compile_options(computer_lib_cov PRIVATE -Wall -Wextra -Wpedantic --coverage -O0 -g)
target_link_options(computer_lib_cov INTERFACE --coverage)

add_executable(computer_tests ${TEST_SOURCES})
target_link_libraries(computer_tests PRIVATE computer_lib_cov Catch2::Catch2WithMain)
target_compile_options(computer_tests PRIVATE --coverage -O0 -g)
