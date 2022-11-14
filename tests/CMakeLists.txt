include(CMakePrintHelpers)

cmake_print_variables(CMAKE_CXX_COMPILER_ID)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-g -O0 -Wall --coverage)
    add_link_options(--coverage)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-g -O0 -Wall --coverage)
    add_link_options(--coverage)
else()
    message("coverage not supported")
endif()

include_directories(../sparsematrix)

add_executable(test_basic test_basic.cpp)
add_executable(test_mult_1d test_mult_1d.cpp)
add_executable(test_mult_2d test_mult_2d.cpp)
add_executable(test_scaling test_scaling.cpp)
add_executable(test_dim_errors test_dim_errors.cpp)

get_property(test_executables DIRECTORY ${WORKING_DIRECTORY} PROPERTY BUILDSYSTEM_TARGETS)
add_custom_target(unittests ALL DEPENDS ${test_executables})
add_custom_command(TARGET unittests POST_BUILD COMMAND ctest WORKING_DIRECTORY ${CMAKE_BINARY_DIR} COMMENT "Running unit tests...")

add_custom_target(coverage ALL DEPENDS unittests)
add_custom_command(TARGET coverage POST_BUILD COMMAND echo "report" WORKING_DIRECTORY ${CMAKE_BINARY_DIR} COMMENT "Generating coverage report...")
