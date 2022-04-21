# -----------------------------------------------------------------------------
# @brief  : Test directory.
# @author : Enrico Fraccaroli
# @create : 03/01/2022
# @update : 03/01/2022
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
# Tests
# -----------------------------------------------------------------------------
project(test_cpptracer CXX)
add_executable(${PROJECT_NAME} test_cpptracer.cpp)
target_link_libraries(${PROJECT_NAME} cpptracer)
target_compile_definitions(${PROJECT_NAME} PRIVATE _USE_MATH_DEFINES)
add_test(NAME run_test_cpptracer COMMAND ${PROJECT_NAME})
