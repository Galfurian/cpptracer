# -----------------------------------------------------------------------------
# @brief  : Root cmake file.
# @author : Enrico Fraccaroli
# @create : 03/01/2022
# @update : 03/01/2022
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
# Set the minimum CMake version, the project name and default build type.
# -----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.1...3.18)

# Set the project name.
project(cpptracer CXX)

# Set the default build type to Debug.
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
endif()

# -----------------------------------------------------------------------------
# Add options.
# -----------------------------------------------------------------------------
option(STRICT_WARNINGS "Enable strict compiler warnings" ON)
option(WARNINGS_AS_ERRORS "Treat all warnings as errors" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)
option(BUILD_TESTS "Build tests" OFF)
option(ENABLE_COMPRESSION "Enables the option to compress VCD traces using zlib" OFF)

# -----------------------------------------------------------------------------
# ENABLE FETCH CONTENT
# -----------------------------------------------------------------------------

# We need this in order to import external projects.
include(FetchContent)
# Hide fetchcontent variables.
mark_as_advanced(FORCE
    FETCHCONTENT_QUIET
    FETCHCONTENT_BASE_DIR
    FETCHCONTENT_FULLY_DISCONNECTED
    FETCHCONTENT_UPDATES_DISCONNECTED
)

# -----------------------------------------------------------------------------
# DEPENDENCY (SYSTEM LIBRARIES)
# -----------------------------------------------------------------------------

find_package(Doxygen)

# -----------------------------------------------------------------------------
# LIBRARY
# -----------------------------------------------------------------------------
# Add the C++ library.
add_library(cpptracer INTERFACE)
add_library(cpptracer::cpptracer ALIAS cpptracer)
# Inlcude header directories.
target_include_directories(cpptracer INTERFACE ${PROJECT_SOURCE_DIR}/include)
# Set compiler flags.
target_compile_features(cpptracer INTERFACE cxx_std_17)
# If compression is enabled.
if(ENABLE_COMPRESSION)
    # Find zlib for traces compression.
    find_package(ZLIB REQUIRED)
    if(ZLIB_FOUND)
        # Link zlib.
        target_link_libraries(cpptracer INTERFACE ZLIB::ZLIB)
        # Add a define inside the code, so that we can activate the compression code.
        target_compile_definitions(cpptracer INTERFACE ENABLE_COMPRESSION)
    endif()
endif()

# =====================================
# COMPILATION FLAGS
# =====================================

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Disable warnings for MSVC-specific "safe" functions like strcpy_s, etc.,
    # which are not portable and may clutter warning logs.
    target_compile_definitions(cpptracer INTERFACE _CRT_SECURE_NO_WARNINGS)

    # Disable warning C4702: unreachable code.
    add_compile_options(/wd4702)

    if(WARNINGS_AS_ERRORS)
        # Treat all warnings as errors to enforce stricter code quality.
        target_compile_options(cpptracer INTERFACE /WX)
    endif()

    if(STRICT_WARNINGS)
        # Enable external header management to suppress warnings in system and
        # external headers, making it easier to focus on project-specific issues.
        target_compile_options(cpptracer INTERFACE /experimental:external)
        target_compile_options(cpptracer INTERFACE /external:I ${CMAKE_BINARY_DIR})
        target_compile_options(cpptracer INTERFACE /external:anglebrackets)
        target_compile_options(cpptracer INTERFACE /external:W0)

        # Use a high warning level to catch as many potential issues as possible.
        target_compile_options(cpptracer INTERFACE /W4)

        # Enforce standards-compliant behavior to avoid relying on MSVC-specific extensions.
        target_compile_options(cpptracer INTERFACE /permissive-)
    endif()

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if(WARNINGS_AS_ERRORS)
        # Treat all warnings as errors to enforce stricter code quality.
        target_compile_options(cpptracer INTERFACE -Werror)
    endif()

    if(STRICT_WARNINGS)
        # Enable a broad set of warnings to catch common and subtle issues:
        target_compile_options(cpptracer INTERFACE
            -Wall                # Enable most general-purpose warnings.
            -Wextra              # Enable extra warnings not included in -Wall.
            -Wconversion         # Warn about implicit type conversions that may lose data.
            -pedantic            # Enforce strict compliance with the C++ standard.
            -Wshadow             # Warn about variable shadowing, which can cause subtle bugs.
            -Wnon-virtual-dtor   # Warn when a class with virtual functions lacks a virtual destructor.
            -Wnull-dereference   # Warn about potential null pointer dereferences.
            -Wformat=2           # Enable strict checks for printf/scanf format strings.
            -Woverloaded-virtual # Warn when a derived class function hides a base class virtual function.
            -Wfloat-equal        # Warn about direct comparisons of floating-point values, which can be imprecise.
        )
    endif()
endif()

# -----------------------------------------------------------------------------
# EXAMPLES
# -----------------------------------------------------------------------------

if(BUILD_EXAMPLES)

    # Add the executable.
    add_executable(cpptracer_compression ${PROJECT_SOURCE_DIR}/examples/compression.cpp)
    target_link_libraries(cpptracer_compression cpptracer)
    target_compile_definitions(cpptracer_compression PRIVATE _USE_MATH_DEFINES)
    
    # Add the executable.
    add_executable(cpptracer_datatypes ${PROJECT_SOURCE_DIR}/examples/datatypes.cpp)
    target_link_libraries(cpptracer_datatypes cpptracer)
    target_compile_definitions(cpptracer_datatypes PRIVATE _USE_MATH_DEFINES)

    # Add the executable.
    add_executable(cpptracer_scope ${PROJECT_SOURCE_DIR}/examples/scope.cpp)
    target_link_libraries(cpptracer_scope cpptracer)
    target_compile_definitions(cpptracer_scope PRIVATE _USE_MATH_DEFINES)

endif()

# -----------------------------------------------------------------------------
# TESTS
# -----------------------------------------------------------------------------

if(BUILD_TESTS)

    # CMake has support for adding tests to a project.
    enable_testing()

    # Add the executable.
    add_executable(cpptracer_test_datatypes ${PROJECT_SOURCE_DIR}/tests/test_datatypes.cpp)
    target_link_libraries(cpptracer_test_datatypes cpptracer)
    target_compile_definitions(cpptracer_test_datatypes PRIVATE _USE_MATH_DEFINES)
    add_test(NAME cpptracer_run_test_datatypes COMMAND cpptracer_test_datatypes)

endif()
# -----------------------------------------------------------------------------
# DOCUMENTATION
# -----------------------------------------------------------------------------

if(DOXYGEN_FOUND)
    
    
    # FetchContent: Doxygen Awesome CSS
    FetchContent_Declare(doxygenawesome
        GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css
        GIT_TAG main
    )
    FetchContent_MakeAvailable(doxygenawesome)

    # Hide FetchContent variables to avoid clutter in ccmake.
    mark_as_advanced(FORCE
        FETCHCONTENT_UPDATES_DISCONNECTED_DOXYGENAWESOME
        FETCHCONTENT_SOURCE_DIR_DOXYGENAWESOME
    )

    # Read the file with the version.
    file(READ ${PROJECT_SOURCE_DIR}/include/cpptracer/tracer.hpp version_file)
    # Extract the version.
    string(REGEX MATCH "_MAJOR_VERSION ([0-9]*)" _ ${version_file})
    set(PROGRAM_MAJOR_VERSION ${CMAKE_MATCH_1})
    string(REGEX MATCH "_MINOR_VERSION ([0-9]*)" _ ${version_file})
    set(PROGRAM_MINOR_VERSION ${CMAKE_MATCH_1})
    string(REGEX MATCH "_MICRO_VERSION ([0-9]*)" _ ${version_file})
    set(PROGRAM_MICRO_VERSION ${CMAKE_MATCH_1})

    # Customization: Doxygen Configuration
    set(DOXYGEN_WARN_FORMAT "$file:$line: $text")
    set(DOXYGEN_PROJECT_NAME cpptracer)
    set(DOXYGEN_PROJECT_BRIEF "A VCD tracer for c++ variables")
    set(DOXYGEN_PROJECT_NUMBER "${PROGRAM_MAJOR_VERSION}.${PROGRAM_MINOR_VERSION}.${PROGRAM_MICRO_VERSION}")
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${PROJECT_SOURCE_DIR}/README.md)
    set(DOXYGEN_SHOW_INCLUDE_FILES NO)
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_GENERATE_LATEX NO) # Disable LaTeX if not needed
    set(DOXYGEN_GENERATE_MAN NO)   # Disable man pages if not needed

    # Styling and UX enhancements using Doxygen Awesome
    set(DOXYGEN_HTML_HEADER ${doxygenawesome_SOURCE_DIR}/doxygen-custom/header.html)
    set(DOXYGEN_HTML_EXTRA_STYLESHEET ${doxygenawesome_SOURCE_DIR}/doxygen-awesome.css)
    set(DOXYGEN_HTML_EXTRA_FILES
        ${doxygenawesome_SOURCE_DIR}/doxygen-awesome-fragment-copy-button.js
        ${doxygenawesome_SOURCE_DIR}/doxygen-awesome-paragraph-link.js
        ${doxygenawesome_SOURCE_DIR}/doxygen-awesome-darkmode-toggle.js
    )

    # Set stricter warnings for better documentation quality
    set(DOXYGEN_WARN_IF_UNDOCUMENTED YES)
    set(DOXYGEN_WARN_IF_DOC_ERROR YES)
    set(DOXYGEN_WARN_NO_PARAMDOC YES)
    set(DOXYGEN_WARN_AS_ERROR YES) # Treat warnings as errors for CI

    # Exclude certain files or directories from documentation (if needed)
    set(DOXYGEN_EXCLUDE_PATTERNS
        "${PROJECT_SOURCE_DIR}/tests/*"
        "${PROJECT_SOURCE_DIR}/examples/*"
    )

    # Add Doxygen documentation target
    doxygen_add_docs(
        cpptracer_documentation
        ${PROJECT_SOURCE_DIR}/README.md
        ${PROJECT_SOURCE_DIR}/LICENSE.md
        ${PROJECT_SOURCE_DIR}/include/cpptracer/colors.hpp
        ${PROJECT_SOURCE_DIR}/include/cpptracer/compression.hpp
        ${PROJECT_SOURCE_DIR}/include/cpptracer/feq.hpp
        ${PROJECT_SOURCE_DIR}/include/cpptracer/timeScale.hpp
        ${PROJECT_SOURCE_DIR}/include/cpptracer/trace.hpp
        ${PROJECT_SOURCE_DIR}/include/cpptracer/tracer.hpp
        ${PROJECT_SOURCE_DIR}/include/cpptracer/utilities.hpp
        COMMENT "Generating Doxygen documentation for the library"
    )

endif()
