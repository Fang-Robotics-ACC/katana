cmake_minimum_required(VERSION 4.2.0)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE REGULAR_SRC ${PROJECT_SOURCE_DIR} src/*.cpp)
file(GLOB_RECURSE REGULAR_MODULE_SRC ${PROJECT_SOURCE_DIR} src/*.cppm)
project(katana CXX)

find_package(GTest REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(
    katana
    ${REGULAR_SRC}
    ${REGULAR_MODULE_SRC}
)

if(MINGW AND CMAKE_CXX_COMPILER_ID STREQUAL GNU)
    target_link_libraries(${PROJECT_NAME} stdc++exp)
endif()

target_sources(
    katana PRIVATE
    FILE_SET all_my_modules TYPE CXX_MODULES
    BASE_DIRS
    ${PROJECT_SOURCE_DIR}
    FILES
    ${REGULAR_MODULE_SRC}
)


set(TESTING_SRC ${REGULAR_SRC})
# prevent main.cpp from hijacking unit test
list(REMOVE_ITEM TESTING_SRC "${PROJECT_SOURCE_DIR}/src/main.cpp" )
file(GLOB_RECURSE TEST_SRC ${PROJECT_SOURCE_DIR} test/*.cpp)

add_executable(
    unit_tests 
    ${TEST_SRC}
    ${TESTING_SRC}
    ${MODULES_SRC}
)

target_sources(
    unit_tests PRIVATE
    FILE_SET all_my_modules TYPE CXX_MODULES
    BASE_DIRS
    ${PROJECT_SOURCE_DIR}
    FILES
    ${REGULAR_MODULE_SRC}
)

target_link_libraries(
    unit_tests 
    GTest::Main
    GTest::GTest
    GTest::gmock
    ${OpenCV_LIBS}
    ${Boost_LIBRARIES}
)

target_include_directories(unit_tests PRIVATE ${PROJECT_SOURCE_DIR}/src)

include(GoogleTest test)
gtest_discover_tests(unit_tests)
