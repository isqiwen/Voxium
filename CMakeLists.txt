cmake_minimum_required(VERSION 3.27.0)

project(
    Voxium
    VERSION 0.1
    DESCRIPTION "Voxel Game Engine"
    HOMEPAGE_URL "%%github.com/isqiwen/Voxium%%"
    LANGUAGES CXX
)

set(VOXIUM_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed. Use a separate build directory.")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CMake/Tools.cmake)
include(CMake/StandardSettings.cmake)
include(CMake/StaticAnalyzers.cmake)
include(CMake/ProjectOptions.cmake)

print_project_configuration()

add_subdirectory(Engine)
