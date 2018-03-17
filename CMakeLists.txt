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