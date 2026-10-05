add_executable(pfe_bench bench.cpp)
target_include_directories(pfe_bench PRIVATE ${PROJECT_SOURCE_DIR}/test)
target_link_libraries(pfe_bench PRIVATE PFE::PFE)
