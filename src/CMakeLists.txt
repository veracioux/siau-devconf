cmake_minimum_required(VERSION 3.10)
project(devconf)

set(SRC_DIR ${CMAKE_SOURCE_DIR})

set(EXECUTABLE devconf)

include(common.cmake)
target_sources(devconf PRIVATE main.cpp)
