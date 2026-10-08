find_package(GTest QUIET)
if (NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG v1.17.0
    )
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif ()
include(GoogleTest)

set(qualified_tags "")
foreach (curve IN LISTS RBP_CURVES)
    string(TOUPPER ${curve} tag)
    list(APPEND qualified_tags rbp::${tag})
endforeach ()
list(JOIN qualified_tags ", " tag_list)
file(CONFIGURE OUTPUT ${PROJECT_BINARY_DIR}/generated/test/curves.hpp CONTENT "#ifndef PFE_TEST_CURVES_HPP
#define PFE_TEST_CURVES_HPP

#include <tuple>
#include <gtest/gtest.h>
#include <rbp/rbp.hpp>

using CurveTuple = std::tuple<${tag_list}>;

#endif
")

foreach (source IN ITEMS test_ipfe.cpp test_qfe.cpp test_concurrency.cpp)
    get_filename_component(name ${source} NAME_WE)
    add_executable(${name} ${source})
    target_include_directories(${name} PRIVATE ${PROJECT_BINARY_DIR}/generated/test)
    target_link_libraries(${name} PRIVATE PFE::PFE GTest::gtest_main)
    gtest_discover_tests(${name})
endforeach ()
