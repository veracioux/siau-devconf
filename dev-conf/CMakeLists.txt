cmake_minimum_required(VERSION 3.10)
project(dev-conf)

add_executable(dev-conf main.cpp)
target_link_libraries(dev-conf jsoncpp)

if (NOT DEFINED TEMPLATE_DIR)
    set(TEMPLATE_DIR "./template")
endif ()

configure_file(symbols.h.in symbols.h @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

unset(TEMPLATE_DIR CACHE)
