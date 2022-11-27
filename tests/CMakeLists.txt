include(CMakePrintHelpers)

cmake_print_variables(CMAKE_CXX_COMPILER)
cmake_print_variables(CMAKE_CXX_COMPILER_ID)
cmake_print_variables(CMAKE_SOURCE_DIR)
cmake_print_variables(CMAKE_BINARY_DIR)
cmake_print_variables(CMAKE_CURRENT_BINARY_DIR)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-g -O0 -Wall -fprofile-instr-generate -fcoverage-mapping)
    add_link_options(-fprofile-instr-generate)
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

get_property(TEST_EXECUTABLES DIRECTORY ${WORKING_DIRECTORY} PROPERTY BUILDSYSTEM_TARGETS)

add_custom_target(unittests ALL DEPENDS ${TEST_EXECUTABLES})
add_custom_command(TARGET unittests POST_BUILD
                   COMMAND ctest
                   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                   COMMENT "Running unit tests...")

add_custom_target(coverage ALL DEPENDS unittests)
add_custom_command(TARGET coverage POST_BUILD
                   COMMAND ${CMAKE_SOURCE_DIR}/bin/collect_coverage ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CURRENT_BINARY_DIR} ${TEST_EXECUTABLES}
                   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                   COMMENT "Generating coverage report...")
