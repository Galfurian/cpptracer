# -----------------------------------------------------------------------------
# @brief  : Root cmake file.
# @author : Enrico Fraccaroli
# @create : 03/01/2022
# @update : 03/01/2022
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
# Set the minimum CMake version, the project name and default build type.
# -----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.1)

# Set the project name.
project(cpptracer CXX)

# -----------------------------------------------------------------------------
# Add options.
# -----------------------------------------------------------------------------
option(CPPTRACER_COMPRESS "Enables the option to compress VCD traces using zlib" OFF)
option(CPPTRACER_STRICT_WARNINGS "Enable strict compiler warnings" ON)
option(CPPTRACER_WARNINGS_AS_ERRORS "Treat all warnings as errors" OFF)
option(CPPTRACER_BUILD_EXAMPLES "Build examples" OFF)

# -----------------------------------------------------------------------------
# Set the compilation flags.
# -----------------------------------------------------------------------------

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Disable warnings that suggest using MSVC-specific safe functions
    set(CPPTRACER_COMPILE_OPTIONS ${CPPTRACER_COMPILE_OPTIONS} -D_CRT_SECURE_NO_WARNINGS)

    if(CPPTRACER_WARNINGS_AS_ERRORS)
        set(CPPTRACER_COMPILE_OPTIONS ${CPPTRACER_COMPILE_OPTIONS} /WX)
    endif(CPPTRACER_WARNINGS_AS_ERRORS)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if(CPPTRACER_WARNINGS_AS_ERRORS)
        set(CPPTRACER_COMPILE_OPTIONS ${CPPTRACER_COMPILE_OPTIONS} -Werror)
    endif(CPPTRACER_WARNINGS_AS_ERRORS)
endif()

if(CPPTRACER_STRICT_WARNINGS)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        # Mark system headers as external for MSVC explicitly
        # https://devblogs.microsoft.com/cppblog/broken-warnings-theory
        set(CPPTRACER_COMPILE_OPTIONS ${CPPTRACER_COMPILE_OPTIONS} /experimental:external)
        set(CPPTRACER_COMPILE_OPTIONS ${CPPTRACER_COMPILE_OPTIONS} /external:I ${CMAKE_BINARY_DIR})
        set(CPPTRACER_COMPILE_OPTIONS ${CPPTRACER_COMPILE_OPTIONS} /external:anglebrackets)
        set(CPPTRACER_COMPILE_OPTIONS ${CPPTRACER_COMPILE_OPTIONS} /external:W0)
        set(CPPTRACER_COMPILE_OPTIONS ${CPPTRACER_COMPILE_OPTIONS} /W4)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CPPTRACER_COMPILE_OPTIONS ${CPPTRACER_COMPILE_OPTIONS} -Wall -Wextra -Wconversion -pedantic)
    endif()
endif(CPPTRACER_STRICT_WARNINGS)

# -----------------------------------------------------------------------------
# LIBRARY
# -----------------------------------------------------------------------------
# Add the C++ library.
add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
# Inlcude header directories.
target_include_directories(${PROJECT_NAME} INTERFACE ${PROJECT_SOURCE_DIR}/include)
# Set compilation flags.
target_compile_options(${PROJECT_NAME} INTERFACE ${CPPTRACER_COMPILE_OPTIONS})
# Set compiler flags.
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
# If compression is enabled.
if(CPPTRACER_COMPRESS)
    # Find zlib for traces compression.
    find_package(ZLIB REQUIRED)
    if(ZLIB_FOUND)
        # Link zlib.
        target_link_libraries(${PROJECT_NAME} INTERFACE ZLIB::ZLIB)
        # Add a define inside the code, so that we can activate the compression code.
        target_compile_definitions(${PROJECT_NAME} INTERFACE COMPRESSION_ENABLED)
    endif()
endif()

# -----------------------------------------------------------------------------
# EXAMPLES
# -----------------------------------------------------------------------------

if(CPPTRACER_BUILD_EXAMPLES)

    # Add the executable.
    add_executable(${PROJECT_NAME}_compression ${PROJECT_SOURCE_DIR}/examples/compression.cpp)
    # Set the linked libraries.
    target_link_libraries(${PROJECT_NAME}_compression ${PROJECT_NAME})
    # Add a defines inside the code.
    target_compile_definitions(${PROJECT_NAME}_compression PRIVATE _USE_MATH_DEFINES)
    # Set compilation flags.
    target_compile_options(${PROJECT_NAME}_compression PRIVATE ${CPPTRACER_COMPILE_OPTIONS})
    # Set compiler flags.
    target_compile_features(${PROJECT_NAME}_compression PRIVATE cxx_std_17)
    
    # Add the executable.
    add_executable(${PROJECT_NAME}_datatypes ${PROJECT_SOURCE_DIR}/examples/datatypes.cpp)
    # Set the linked libraries.
    target_link_libraries(${PROJECT_NAME}_datatypes ${PROJECT_NAME})
    # Add a defines inside the code.
    target_compile_definitions(${PROJECT_NAME}_datatypes PRIVATE _USE_MATH_DEFINES)
    # Set compilation flags.
    target_compile_options(${PROJECT_NAME}_datatypes PRIVATE ${CPPTRACER_COMPILE_OPTIONS})
    # Set compiler flags.
    target_compile_features(${PROJECT_NAME}_datatypes PRIVATE cxx_std_17)

    # Add the executable.
    add_executable(${PROJECT_NAME}_scope ${PROJECT_SOURCE_DIR}/examples/scope.cpp)
    # Set the linked libraries.
    target_link_libraries(${PROJECT_NAME}_scope ${PROJECT_NAME})
    # Add a defines inside the code.
    target_compile_definitions(${PROJECT_NAME}_scope PRIVATE _USE_MATH_DEFINES)
    # Set compilation flags.
    target_compile_options(${PROJECT_NAME}_scope PRIVATE ${CPPTRACER_COMPILE_OPTIONS})
    # Set compiler flags.
    target_compile_features(${PROJECT_NAME}_scope PRIVATE cxx_std_17)

endif()

# -----------------------------------------------------------------------------
# TESTS
# -----------------------------------------------------------------------------

enable_testing()

# Add the executable.
add_executable(${PROJECT_NAME}_test_datatypes ${PROJECT_SOURCE_DIR}/tests/test_datatypes.cpp)
# Set the linked libraries.
target_link_libraries(${PROJECT_NAME}_test_datatypes ${PROJECT_NAME})
# Add a defines inside the code.
target_compile_definitions(${PROJECT_NAME}_test_datatypes PRIVATE _USE_MATH_DEFINES)
# Add tje test.
add_test(NAME ${PROJECT_NAME}_run_test_datatypes COMMAND ${PROJECT_NAME}_test_datatypes)

# -----------------------------------------------------------------------------
# DOCUMENTATION
# -----------------------------------------------------------------------------

# Find doxygen.
find_package(Doxygen)
# If we have doxygen, enable the documentation target.
if(DOXYGEN_FOUND)
    
    # = RETRIVAL ==============================================================
    # Include fetch content.
    include(FetchContent)
    # Record the options that describe how to populate the specified content.
    FetchContent_Declare(
        doxygenawesome
        GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css
        GIT_TAG 4cd62308d825fe0396d2f66ffbab45d0e247724c # 2.0.3
    )
    # Retrieve the properties related to the content.
    FetchContent_GetProperties(doxygenawesome)
    # If not populated, make the content available.
    if(NOT doxygenawesome_POPULATED)
        message(STATUS "Retrieving `doxygen-awesome-css`...")
        # Ensures the named dependencies have been populated.
        FetchContent_MakeAvailable(doxygenawesome)
        # Hide fetchcontent variables, otherwise with ccmake it's a mess.
        mark_as_advanced(FORCE
            FETCHCONTENT_QUIET FETCHCONTENT_BASE_DIR FETCHCONTENT_FULLY_DISCONNECTED FETCHCONTENT_UPDATES_DISCONNECTED
            FETCHCONTENT_UPDATES_DISCONNECTED_DOXYGENAWESOME FETCHCONTENT_SOURCE_DIR_DOXYGENAWESOME
        )
    endif()

    # = CUSTOMIZATION =========================================================
    set(DOXYGEN_PROJECT_NAME "CPPTracer")
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${PROJECT_SOURCE_DIR}/README.md)
    set(DOXYGEN_SHOW_INCLUDE_FILES NO)
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_WARN_FORMAT "$file:$line: $text")
    set(DOXYGEN_HTML_HEADER ${doxygenawesome_SOURCE_DIR}/doxygen-custom/header.html)
    set(DOXYGEN_HTML_EXTRA_STYLESHEET ${doxygenawesome_SOURCE_DIR}/doxygen-awesome.css)
    set(DOXYGEN_HTML_EXTRA_FILES
        ${doxygenawesome_SOURCE_DIR}/doxygen-awesome-fragment-copy-button.js
        ${doxygenawesome_SOURCE_DIR}/doxygen-awesome-paragraph-link.js
        ${doxygenawesome_SOURCE_DIR}/doxygen-awesome-darkmode-toggle.js
    )
    doxygen_add_docs(
        ${PROJECT_NAME}_documentation
        ${PROJECT_SOURCE_DIR}/README.md
        ${PROJECT_SOURCE_DIR}/LICENSE.md
        ${PROJECT_SOURCE_DIR}/include/cpptracer/colors.hpp
        ${PROJECT_SOURCE_DIR}/include/cpptracer/compression.hpp
        ${PROJECT_SOURCE_DIR}/include/cpptracer/feq.hpp
        ${PROJECT_SOURCE_DIR}/include/cpptracer/timeScale.hpp
        ${PROJECT_SOURCE_DIR}/include/cpptracer/trace.hpp
        ${PROJECT_SOURCE_DIR}/include/cpptracer/tracer.hpp
        ${PROJECT_SOURCE_DIR}/include/cpptracer/utilities.hpp
        ${PROJECT_SOURCE_DIR}/include/cpptracer/version.hpp
    )
endif()
