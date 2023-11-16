# -----------------------------------------------------
add_executable(test_sym_init test_sym_init.cpp)
target_link_libraries(test_sym_init PRIVATE sym_fe_lib)
add_test(NAME test_sym_init COMMAND test_sym_init)

# -----------------------------------------------------
add_executable(test_sym_field test_sym_field.cpp)
target_link_libraries(test_sym_field PRIVATE sym_fe_lib)
add_test(NAME test_sym_field COMMAND test_sym_field)

# -----------------------------------------------------
add_executable(test_sym_group test_sym_group.cpp)
target_link_libraries(test_sym_group PRIVATE sym_fe_lib)
add_test(NAME test_sym_group COMMAND test_sym_group)

# -----------------------------------------------------
add_executable(test_sym_vector test_sym_vector.cpp)
target_link_libraries(test_sym_vector PRIVATE sym_fe_lib)
add_test(NAME test_sym_vector COMMAND test_sym_vector)

# -----------------------------------------------------
add_executable(test_sym_matrix test_sym_matrix.cpp)
target_link_libraries(test_sym_matrix PRIVATE sym_fe_lib)
add_test(NAME test_sym_matrix COMMAND test_sym_matrix)

# -----------------------------------------------------
add_executable(test_sym_ipre test_sym_ipre.cpp)
target_link_libraries(test_sym_ipre PRIVATE sym_fe_lib)
add_test(NAME test_sym_ipre COMMAND test_sym_ipre)

# -----------------------------------------------------
add_executable(test_asym_init test_asym_init.cpp)
target_link_libraries(test_asym_init PRIVATE asym_fe_lib)
add_test(NAME test_asym_init COMMAND test_asym_init)

# -----------------------------------------------------
add_executable(test_asym_field test_asym_field.cpp)
target_link_libraries(test_asym_field PRIVATE asym_fe_lib)
add_test(NAME test_asym_field COMMAND test_asym_field)

# -----------------------------------------------------
add_executable(test_asym_group test_asym_group.cpp)
target_link_libraries(test_asym_group PRIVATE asym_fe_lib)
add_test(NAME test_asym_group COMMAND test_asym_group)

# -----------------------------------------------------
add_executable(test_asym_vector test_asym_vector.cpp)
target_link_libraries(test_asym_vector PRIVATE asym_fe_lib)
add_test(NAME test_asym_vector COMMAND test_asym_vector)

# -----------------------------------------------------
add_executable(test_asym_matrix test_asym_matrix.cpp)
target_link_libraries(test_asym_matrix PRIVATE asym_fe_lib)
add_test(NAME test_asym_matrix COMMAND test_asym_matrix)

# -----------------------------------------------------
add_executable(test_asym_ipfe test_asym_ipfe.cpp)
target_link_libraries(test_asym_ipfe PRIVATE asym_fe_lib)
add_test(NAME test_asym_ipfe COMMAND test_asym_ipfe)



## Add tests as separate executables.
#add_executable(test_field_sym test_field_sym.cpp)
#add_executable(test_group test_group.cpp)
#add_executable(test_vector test_vector.cpp)
#add_executable(test_matrix test_matrix.cpp)
#
#add_executable(test_field_asym test_field_asym.cpp)
#add_executable(test_group_asym test_group_asym.cpp)
#
#
#add_executable(test_ipre test_ipre.cpp)
#add_executable(test_helper test_helper.cpp)
#add_executable(test_hnsw test_hnsw.cpp)
#
## Link tests to the main library.
#target_link_libraries(test_field_sym PRIVATE ppann_lib)
#target_link_libraries(test_group PRIVATE ppann_lib)
#target_link_libraries(test_vector PRIVATE ppann_lib)
#target_link_libraries(test_matrix PRIVATE ppann_lib)
#
#target_link_libraries(test_field_asym PRIVATE ppann_lib)
#target_link_libraries(test_group_asym PRIVATE ppann_lib)
#
#target_link_libraries(test_ipre PRIVATE ppann_lib)
#target_link_libraries(test_helper PRIVATE ppann_lib)
#target_link_libraries(test_hnsw PRIVATE ppann_lib)
#
## Register the previous tests.
#add_test(NAME test_field_sym COMMAND test_field_sym)
#add_test(NAME test_group COMMAND test_group)
#add_test(NAME test_vector COMMAND test_vector)
#add_test(NAME test_matrix COMMAND test_matrix)
#
#add_test(NAME test_field_asym COMMAND test_field_asym)
#add_test(NAME test_group_asym COMMAND test_group_asym)
#
#add_test(NAME test_ipre COMMAND test_ipre)
#add_test(NAME test_helper COMMAND test_helper WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/data")
#add_test(NAME test_hnsw COMMAND test_hnsw WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/data")