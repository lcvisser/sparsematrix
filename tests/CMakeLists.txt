add_executable(test_basic test_basic.cpp)
target_include_directories(test_basic PRIVATE ../sparsematrix)

add_executable(test_mult_1d test_mult_1d.cpp)
target_include_directories(test_mult_1d PRIVATE ../sparsematrix)

add_executable(test_mult_2d test_mult_2d.cpp)
target_include_directories(test_mult_2d PRIVATE ../sparsematrix)

add_executable(test_dim_errors test_dim_errors.cpp)
target_include_directories(test_dim_errors PRIVATE ../sparsematrix)
