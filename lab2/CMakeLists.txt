cmake_minimum_required(VERSION 3.16)
project(TravelAgency)

file(GLOB TRAVEL_SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_library(travel_lib ${TRAVEL_SOURCES})
target_include_directories(travel_lib PUBLIC include)
target_compile_options(travel_lib PRIVATE -Wall -Wextra -Wpedantic)


add_executable(travel_cli cli/main.cpp)
target_link_libraries(travel_cli PRIVATE travel_lib)
target_compile_options(travel_cli PRIVATE -Wall -Wextra -Wpedantic)

find_package(Catch2 REQUIRED)
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS tests/*.cpp)

add_library(travel_lib_cov STATIC ${TRAVEL_SOURCES})
target_include_directories(travel_lib_cov PUBLIC include)
target_compile_options(travel_lib_cov PRIVATE -Wall -Wextra -Wpedantic --coverage -O0 -g)
target_link_options(travel_lib_cov INTERFACE --coverage)

add_executable(travel_tests ${TEST_SOURCES})
target_link_libraries(travel_tests PRIVATE travel_lib_cov Catch2::Catch2WithMain)
target_compile_options(travel_tests PRIVATE --coverage -O0 -g)