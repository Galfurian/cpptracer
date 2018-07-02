# -----------------------------------------------------------------------------
# Check CMAKE Version.
cmake_minimum_required(VERSION 2.8.4 FATAL_ERROR)

# -----------------------------------------------------------------------------
# Set the project name.
SET(PROJECT_NAME cpptracer)

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
include_directories(include)

# -----------------------------------------------------------------------------
# Add test directory.
add_subdirectory(tests)
enable_testing()
add_test(NAME ProjectTest COMMAND ProjectTest "test")

# -----------------------------------------------------------------------------
# Add the documentation generation.
find_package(Doxygen)
if (DOXYGEN_FOUND)
    add_custom_target(doc
            ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/doc/project.doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif (DOXYGEN_FOUND)