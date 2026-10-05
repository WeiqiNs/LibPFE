cmake_minimum_required(VERSION 3.25)

project(
        LibPFE
        VERSION 0.1
        DESCRIPTION "Pairing-based functional encryption schemes over LibRBP curves."
        LANGUAGES CXX
)

set(CMAKE_CXX_EXTENSIONS OFF)
if (PROJECT_IS_TOP_LEVEL AND NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif ()

option(PFE_BUILD_TESTS "Build the LibPFE test suite" ${PROJECT_IS_TOP_LEVEL})
option(PFE_ENABLE_COVERAGE "Build the LibPFE tests with --coverage" OFF)
option(PFE_BUILD_BENCH "Build the LibPFE benchmark" OFF)

include(GNUInstallDirs)
include(FetchContent)
FetchContent_Declare(RBP
        GIT_REPOSITORY https://github.com/WeiqiNs/LibRBP.git
        GIT_TAG main
        GIT_SHALLOW TRUE
        FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(RBP)

add_library(PFE INTERFACE)
add_library(PFE::PFE ALIAS PFE)
target_include_directories(PFE INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(PFE INTERFACE RBP::RBP)

include(cmake/PFEInstall.cmake)

if (PFE_BUILD_TESTS)
    if (PFE_ENABLE_COVERAGE)
        add_compile_options(--coverage -O0 -g)
        add_link_options(--coverage)
    endif ()
    enable_testing()
    add_subdirectory(test)
endif ()

if (PFE_BUILD_BENCH)
    add_subdirectory(bench)
endif ()
