cmake_minimum_required(VERSION 3.10)

project(sparsematrix VERSION 0.1)
enable_testing()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(tests testbin)

foreach(TEST_EXE test_basic test_mult_1d test_mult_2d test_scaling test_dim_errors)
    message(STATUS "Adding test ${TEST_EXE}")
    add_test(NAME ${TEST_EXE}
             COMMAND ${PROJECT_SOURCE_DIR}/bin/run_test_with_coverage ${CMAKE_CXX_COMPILER_ID} $<TARGET_FILE:${TEST_EXE}>)
endforeach()
