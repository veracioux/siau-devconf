cmake_minimum_required(VERSION 3.10)
project(devconf)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Executables
file(GLOB SOURCE_FILES *.cpp)
add_executable(devconf ${SOURCE_FILES})

# Dependencies
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/..)

find_package(Qt5 COMPONENTS Core REQUIRED)

target_link_libraries(devconf Qt5::Core)
add_subdirectory(../devlib ${CMAKE_CURRENT_BINARY_DIR}/dxflib)
target_link_libraries(devconf devlib) 

# Configuring files
if (NOT DEFINED TEMPLATE_DIR)
    set(TEMPLATE_DIR "./template")
endif ()

configure_file(symbols.h.in symbols.h @ONLY)

unset(TEMPLATE_DIR CACHE)

