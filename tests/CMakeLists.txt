# -----------------------------------------------------------------------------
# Check CMAKE Version.
cmake_minimum_required(VERSION 2.8.4 FATAL_ERROR)

# -----------------------------------------------------------------------------
# Set the project name.
SET(PROJECT_NAME ProjectTest)

# -----------------------------------------------------------------------------
# Create the project.
project(${PROJECT_NAME} CXX)

# -----------------------------------------------------------------------------
# Initialize CXXFLAGS.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wconversion")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

# -----------------------------------------------------------------------------
# Set inlcuded directories.
include_directories(${CMAKE_SOURCE_DIR}/include)

# -----------------------------------------------------------------------------
# Add the target library.
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME})
