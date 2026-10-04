include(CMakePackageConfigHelpers)

install(TARGETS RIPFE EXPORT RIPFETargets)
install(DIRECTORY include/RIPFE DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
set(RIPFE_CMAKE_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/RIPFE)
install(EXPORT RIPFETargets NAMESPACE RIPFE:: DESTINATION ${RIPFE_CMAKE_INSTALL_DIR})
configure_package_config_file(cmake/RIPFEConfig.cmake.in RIPFEConfig.cmake
        INSTALL_DESTINATION ${RIPFE_CMAKE_INSTALL_DIR}
)
write_basic_package_version_file(RIPFEConfigVersion.cmake COMPATIBILITY SameMinorVersion ARCH_INDEPENDENT)
install(FILES ${PROJECT_BINARY_DIR}/RIPFEConfig.cmake ${PROJECT_BINARY_DIR}/RIPFEConfigVersion.cmake
        DESTINATION ${RIPFE_CMAKE_INSTALL_DIR}
)
