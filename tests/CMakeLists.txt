include(CMakePrintHelpers)

cmake_print_variables(CMAKE_CXX_COMPILER)
cmake_print_variables(CMAKE_CXX_COMPILER_ID)
cmake_print_variables(CMAKE_SOURCE_DIR)
cmake_print_variables(CMAKE_BINARY_DIR)
cmake_print_variables(CMAKE_CURRENT_BINARY_DIR)

include_directories(../sparsematrix)

add_executable(test_basic test_basic.cpp)
add_executable(test_mult_1d test_mult_1d.cpp)
add_executable(test_mult_2d test_mult_2d.cpp)
add_executable(test_scaling test_scaling.cpp)
add_executable(test_dim_errors test_dim_errors.cpp)
