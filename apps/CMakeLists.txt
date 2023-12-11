# Shows example usage of the two libraries. They should not be linked to the same executable.
add_executable(sym_app sym_app.cpp)
target_link_libraries(sym_app PRIVATE sym_fe_lib)
add_executable(asym_app asym_app.cpp)
target_link_libraries(asym_app PRIVATE asym_fe_lib)
add_executable(multi_thread multi_thread.cpp)
target_link_libraries(multi_thread PRIVATE asym_fe_lib)