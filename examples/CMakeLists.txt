# -----------------------------------------------------------------------------
# @brief  : Examples directory.
# @author : Enrico Fraccaroli
# @create : 03/01/2022
# @update : 03/01/2022
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
# Example 1
# -----------------------------------------------------------------------------
project(compression CXX)
add_executable(compression compression.cpp)
target_link_libraries(compression cpptracer)
target_compile_definitions(compression PRIVATE _USE_MATH_DEFINES)

# -----------------------------------------------------------------------------
# Example 2
# -----------------------------------------------------------------------------
project(datatypes CXX)
add_executable(datatypes datatypes.cpp)
target_link_libraries(datatypes cpptracer)
target_compile_definitions(datatypes PRIVATE _USE_MATH_DEFINES)

# -----------------------------------------------------------------------------
# Example 3
# -----------------------------------------------------------------------------
project(scope CXX)
add_executable(scope scope.cpp)
target_link_libraries(scope cpptracer)
target_compile_definitions(scope PRIVATE _USE_MATH_DEFINES)

