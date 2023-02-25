set(HEADER_LIST "${CMAKE_SOURCE_DIR}/include/")

add_library(ppann_lib field.c group.c vector.c matrix.c ${HEADER_LIST})

# We need this directory, and users of our library will need it too
target_include_directories(ppann_lib PUBLIC ../include)

# This depends on (header only) boost
target_link_libraries(ppann_lib PRIVATE ${RELIC_LIB})

# IDEs should put the headers in a nice place
source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST}
)
