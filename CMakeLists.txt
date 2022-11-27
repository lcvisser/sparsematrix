cmake_minimum_required(VERSION 3.10)

project(sparsematrix VERSION 0.1)
enable_testing()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(tests testbin)
add_test(NAME basic
         COMMAND ${PROJECT_SOURCE_DIR}/bin/run_test_with_coverage ${CMAKE_CXX_COMPILER_ID} $<TARGET_FILE:test_basic> )
add_test(NAME multiplication_1d
         COMMAND ${PROJECT_SOURCE_DIR}/bin/run_test_with_coverage ${CMAKE_CXX_COMPILER_ID} $<TARGET_FILE:test_mult_1d>)
add_test(NAME multiplication_2d
         COMMAND ${PROJECT_SOURCE_DIR}/bin/run_test_with_coverage ${CMAKE_CXX_COMPILER_ID} $<TARGET_FILE:test_mult_2d>)
add_test(NAME scaling
         COMMAND ${PROJECT_SOURCE_DIR}/bin/run_test_with_coverage ${CMAKE_CXX_COMPILER_ID} $<TARGET_FILE:test_scaling>)
add_test(NAME dimension_checks
         COMMAND ${PROJECT_SOURCE_DIR}/bin/run_test_with_coverage ${CMAKE_CXX_COMPILER_ID} $<TARGET_FILE:test_dim_errors>)
