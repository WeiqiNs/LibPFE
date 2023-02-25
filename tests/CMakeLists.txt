# Add tests as separate executables.
add_executable(test_field test_field.c)
add_executable(test_group test_group.c)
add_executable(test_vector test_vector.c)
add_executable(test_matrix test_matrix.c)

# Link tests to the main library.
target_link_libraries(test_field PRIVATE ppann_lib)
target_link_libraries(test_group PRIVATE ppann_lib)
target_link_libraries(test_vector PRIVATE ppann_lib)
target_link_libraries(test_matrix PRIVATE ppann_lib)

# Register the previous tests.
add_test(NAME test_field COMMAND test_field)
add_test(NAME test_group COMMAND test_group)
add_test(NAME test_vector COMMAND test_vector)
add_test(NAME test_matrix COMMAND test_matrix)