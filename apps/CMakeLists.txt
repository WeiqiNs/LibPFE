# Call this executable app.
add_executable(app app.cpp)
target_link_libraries(app PRIVATE ppann_lib)