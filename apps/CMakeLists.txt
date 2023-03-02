# Call this executable app.
add_executable(app scheme.c)
target_link_libraries(app PRIVATE ppann_lib)