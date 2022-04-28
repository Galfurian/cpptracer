# -----------------------------------------------------------------------------
# @brief  : Tests directory.
# @author : Enrico Fraccaroli
# @create : 03/01/2022
# @update : 03/01/2022
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
# Test 1
# -----------------------------------------------------------------------------
project(test_datatypes CXX)
add_executable(test_datatypes test_datatypes.cpp)
target_link_libraries(test_datatypes cpptracer)
target_compile_definitions(test_datatypes PRIVATE _USE_MATH_DEFINES)
add_test(NAME run_test_datatypes COMMAND test_datatypes)
