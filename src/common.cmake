# This file contains the rules to build the devconf executable, and also as a
# library to compile into the unit tests executable

# Requires SRC_DIR to be set to the dir that contains the main executable source
# Requires EXECUTABLE to be set to the name of the executable being built

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core REQUIRED)
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${SRC_DIR}/.. ${SRC_DIR})

# Source files
add_executable(${EXECUTABLE} ${SRC_DIR}/file.cpp ${SRC_DIR}/generator.cpp)

# Dependencies
add_subdirectory(${SRC_DIR}/../devlib ${CMAKE_CURRENT_BINARY_DIR}/devlib)
target_link_libraries(${EXECUTABLE} Qt5::Core devlib)

# Configuring files
if (NOT DEFINED TEMPLATE_DIR)
    set(TEMPLATE_DIR "./template")
endif ()

target_compile_definitions(${EXECUTABLE} PRIVATE TEMPLATE_DIR="${TEMPLATE_DIR}")

unset(TEMPLATE_DIR CACHE)

