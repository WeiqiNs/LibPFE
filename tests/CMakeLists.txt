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

# -----------------------------------------------------
add_executable(test_asym_ipfe_kim test_asym_ipfe_kim.cpp)
target_link_libraries(test_asym_ipfe_kim PRIVATE asym_fe_lib)
add_test(NAME test_asym_ipfe_kim COMMAND test_asym_ipfe_kim)

# -----------------------------------------------------
add_executable(test_asym_ipfe_tao test_asym_ipfe_tao.cpp)
target_link_libraries(test_asym_ipfe_tao PRIVATE asym_fe_lib)
add_test(NAME test_asym_ipfe_tao COMMAND test_asym_ipfe_tao)