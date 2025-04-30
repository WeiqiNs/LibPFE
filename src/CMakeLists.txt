# Collect all source files.
file(GLOB SOURCE_LIST CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Create the library.
add_library(RIPFE ${SOURCE_LIST})

# Create a target alias.
add_library(RIPFE::RIPFE ALIAS RIPFE)

# Set include directories for consumers.
target_include_directories(RIPFE PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Correctly include the RBP library.
target_link_libraries(RIPFE PUBLIC RBP::RBP)

# Set install paths.
set_target_properties(RIPFE PROPERTIES
        INSTALL_RPATH "$ORIGIN/../lib"
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH_USE_LINK_PATH TRUE
)
