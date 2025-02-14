include_guard()

# Create an interface library for project options
add_library(ProjectOptions INTERFACE)

set(INTERNAL_SHARED_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src/")
set(EXTERNAL_SHARED_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src/")

target_include_directories(ProjectOptions INTERFACE
    $<BUILD_INTERFACE:${INTERNAL_SHARED_INCLUDE_DIR}>
    $<BUILD_INTERFACE:${EXTERNAL_SHARED_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# Enable compiler flags
include(cmake/PlatformSettings.cmake)
set_platform_general_settings(ProjectOptions)

# Enable compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(ProjectOptions)

# Include GNU standard installation module
include(GNUInstallDirs)

# Install project options library and export as set
install(TARGETS
    ProjectOptions
    EXPORT ProjectOptionsExportSet
)

# Install the project options export set
install(EXPORT ProjectOptionsExportSet
    FILE ProjectOptionsTargets.cmake
    NAMESPACE ProjectOptions::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ProjectOptions
)
