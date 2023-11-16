# Call this executable app.
add_executable(app app.cpp)
target_link_libraries(app PRIVATE sym_fe_lib asym_fe_lib)