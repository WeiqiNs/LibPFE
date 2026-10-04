cmake_minimum_required(VERSION 3.25)

project(
        LibRIPFE
        VERSION 0.1
        DESCRIPTION "Function-hiding inner-product functional encryption over LibRBP curves."
        LANGUAGES CXX
)

set(CMAKE_CXX_EXTENSIONS OFF)
if (PROJECT_IS_TOP_LEVEL AND NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif ()

option(RIPFE_BUILD_TESTS "Build the LibRIPFE test suite" ${PROJECT_IS_TOP_LEVEL})
option(RIPFE_ENABLE_COVERAGE "Build the LibRIPFE tests with --coverage" OFF)

include(GNUInstallDirs)
include(FetchContent)
FetchContent_Declare(RBP
        GIT_REPOSITORY https://github.com/WeiqiNs/LibRBP.git
        GIT_TAG main
        GIT_SHALLOW TRUE
        FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(RBP)

add_library(RIPFE INTERFACE)
add_library(RIPFE::RIPFE ALIAS RIPFE)
target_include_directories(RIPFE INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(RIPFE INTERFACE RBP::RBP)

include(cmake/RIPFEInstall.cmake)

if (RIPFE_BUILD_TESTS)
    if (RIPFE_ENABLE_COVERAGE)
        add_compile_options(--coverage -O0 -g)
        add_link_options(--coverage)
    endif ()
    enable_testing()
    add_subdirectory(test)
endif ()
