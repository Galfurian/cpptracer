# -----------------------------------------------------------------------------
# @brief  : Test directory.
# @author : Enrico Fraccaroli
# @create : 03/01/2022
# @update : 03/01/2022
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
# Tests
# -----------------------------------------------------------------------------
add_executable(test_cpptracer test_cpptracer.cpp)
target_link_libraries(test_cpptracer cpptracer)
add_test(NAME run_test_cpptracer COMMAND test_cpptracer)
