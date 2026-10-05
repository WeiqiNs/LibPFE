include(CMakePackageConfigHelpers)

install(TARGETS PFE EXPORT PFETargets)
install(DIRECTORY include/PFE DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
set(PFE_CMAKE_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/PFE)
install(EXPORT PFETargets NAMESPACE PFE:: DESTINATION ${PFE_CMAKE_INSTALL_DIR})
configure_package_config_file(cmake/PFEConfig.cmake.in PFEConfig.cmake
        INSTALL_DESTINATION ${PFE_CMAKE_INSTALL_DIR}
)
write_basic_package_version_file(PFEConfigVersion.cmake COMPATIBILITY SameMinorVersion ARCH_INDEPENDENT)
install(FILES ${PROJECT_BINARY_DIR}/PFEConfig.cmake ${PROJECT_BINARY_DIR}/PFEConfigVersion.cmake
        DESTINATION ${PFE_CMAKE_INSTALL_DIR}
)
