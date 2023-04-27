# Add tests as separate executables.
add_executable(test_field test_field.cpp)
add_executable(test_group test_group.cpp)
add_executable(test_vector test_vector.cpp)
add_executable(test_matrix test_matrix.cpp)
add_executable(test_ipre test_ipre.cpp)
add_executable(test_helper test_helper.cpp)
add_executable(test_hnsw test_hnsw.cpp)

# Link tests to the main library.
target_link_libraries(test_field PRIVATE ppann_lib)
target_link_libraries(test_group PRIVATE ppann_lib)
target_link_libraries(test_vector PRIVATE ppann_lib)
target_link_libraries(test_matrix PRIVATE ppann_lib)
target_link_libraries(test_ipre PRIVATE ppann_lib)
target_link_libraries(test_helper PRIVATE ppann_lib)
target_link_libraries(test_hnsw PRIVATE ppann_lib)

# Register the previous tests.
add_test(NAME test_field COMMAND test_field)
add_test(NAME test_group COMMAND test_group)
add_test(NAME test_vector COMMAND test_vector)
add_test(NAME test_matrix COMMAND test_matrix)
add_test(NAME test_ipre COMMAND test_ipre)
add_test(NAME test_helper COMMAND test_helper WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/data")
add_test(NAME test_hnsw COMMAND test_hnsw WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/data")