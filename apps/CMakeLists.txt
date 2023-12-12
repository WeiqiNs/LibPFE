# Shows example usage of the two libraries. They should not be linked to the same executable.
add_executable(sym_demo sym_demo.cpp)
target_link_libraries(sym_demo PRIVATE sym_fe_lib)
add_executable(asym_demo asym_demo.cpp)
target_link_libraries(asym_demo PRIVATE asym_fe_lib)