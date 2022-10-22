cmake_minimum_required(VERSION 3.10)

project(sparsematrix VERSION 0.1)
enable_testing()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(tests test)
add_test(NAME basic COMMAND test_basic)
add_test(NAME multiplication_1d COMMAND test_mult_1d)
add_test(NAME multiplication_2d COMMAND test_mult_2d)
add_test(NAME scaling COMMAND test_scaling)
add_test(NAME dimension_checks COMMAND test_dim_errors)
