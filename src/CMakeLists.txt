# Set parameters for file locations.
set(HEADER_PATH "${CMAKE_SOURCE_DIR}/include/")
set(SOURCE_PATH "${CMAKE_SOURCE_DIR}/src/")

# ----------------------------------------------------------------------
# Set the sym lib.
file(GLOB SYM_HEADER "${HEADER_PATH}sym*.hpp")
file(GLOB SYM_SOURCE "${SOURCE_PATH}sym*.cpp")

# Add sym files to the library.
add_library(sym_fe_lib ${SYM_HEADER} ${SYM_SOURCE})

# We need this directory, and users of our library will need it too
target_include_directories(sym_fe_lib PUBLIC ../include)

# This depends on the relic sym library.
target_link_libraries(sym_fe_lib PRIVATE ${RELIC_LIB_SYM})

# ----------------------------------------------------------------------
# Now do the same for asym ones.
file(GLOB ASYM_HEADER "${HEADER_PATH}asym*.hpp")
file(GLOB ASYM_SOURCE "${SOURCE_PATH}asym*.cpp")

# Add sym files to the library.
add_library(asym_fe_lib ${ASYM_HEADER} ${ASYM_SOURCE})

# We need this directory, and users of our library will need it too
target_include_directories(asym_fe_lib PUBLIC ../include)

# This depends on the relic sym library.
target_link_libraries(asym_fe_lib PRIVATE ${RELIC_LIB_ASYM})

# ----------------------------------------------------------------------
# IDEs should put the headers in a nice place.
source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST}
)
