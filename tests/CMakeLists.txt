cmake_minimum_required(VERSION 3.10)
project(unittests LANGUAGES CXX)

# Configuration
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgcov")
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)

# Variables
set(QT_VERSION_MAJOR 5)
set(QT_VERSION 5.9)
set(EXECUTABLE unittests)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/../src)

# Find QtTest library
find_package(Qt${QT_VERSION_MAJOR} ${QT_VERSION} COMPONENTS Test REQUIRED)

include_directories(${SRC_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

enable_testing(true)
add_test(unittests unittests)

include(${SRC_DIR}/common.cmake)
target_sources(unittests PRIVATE main.cpp)
target_link_libraries(unittests Qt5::Test)
