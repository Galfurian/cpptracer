# -----------------------------------------------------------------------------
# @brief  : Test directory.
# @author : Enrico Fraccaroli
# @create : 03/01/2022
# @update : 03/01/2022
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
# Test 1
# -----------------------------------------------------------------------------
project(test_compression CXX)
add_executable(test_compression test_compression.cpp)
target_link_libraries(test_compression cpptracer)
target_compile_definitions(test_compression PRIVATE _USE_MATH_DEFINES)
add_test(NAME run_test_compression COMMAND test_compression)

# -----------------------------------------------------------------------------
# Test 2
# -----------------------------------------------------------------------------
project(test_datatypes CXX)
add_executable(test_datatypes test_datatypes.cpp)
target_link_libraries(test_datatypes cpptracer)
target_compile_definitions(test_datatypes PRIVATE _USE_MATH_DEFINES)
add_test(NAME run_test_datatypes COMMAND test_datatypes)

# -----------------------------------------------------------------------------
# Test 3
# -----------------------------------------------------------------------------
project(test_scope CXX)
add_executable(test_scope test_scope.cpp)
target_link_libraries(test_scope cpptracer)
target_compile_definitions(test_scope PRIVATE _USE_MATH_DEFINES)
add_test(NAME run_test_scope COMMAND test_scope)

