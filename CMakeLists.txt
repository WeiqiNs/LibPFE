cmake_minimum_required(VERSION 3.24)
project(
        PPANN
        VERSION 0.1
        DESCRIPTION "Test"
        LANGUAGES C
)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_C_STANDARD 17)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    include(CTest)
endif()


find_library(RELIC_LIB relic)

add_subdirectory(src)
add_subdirectory(apps)
add_subdirectory(tests)