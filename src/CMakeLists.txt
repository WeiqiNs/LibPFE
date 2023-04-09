# Include the header list.
set(HEADER_LIST "${CMAKE_SOURCE_DIR}/include/")

# Add all files to the library.
add_library(ppann_lib field.cpp group.cpp vector.cpp matrix.cpp ipre.cpp ${HEADER_LIST})

# We need this directory, and users of our library will need it too
target_include_directories(ppann_lib PUBLIC ../include)

# This depends on the relic library.
target_link_libraries(ppann_lib PRIVATE ${RELIC_LIB})

# IDEs should put the headers in a nice place.
source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST}
)
