#
# Print a message only if the `VERBOSE_OUTPUT` option is on
#

function(verbose_message content)
    if(${PROJECT_NAME}_VERBOSE_OUTPUT)
        message(STATUS ${content})
    endif()
endfunction()

#
# Add a target for formating the project using `clang-format` (i.e: cmake --build build --target clang-format)
#

function(add_cmake_format_target)
    if(NOT ${ENABLE_CMAKE_FORMAT})
        return()
    endif()
    set(ROOT_CMAKE_FILES "${CMAKE_SOURCE_DIR}/CMakeLists.txt")
    file(GLOB_RECURSE CMAKE_FILES_TXT "*/CMakeLists.txt")
    file(GLOB_RECURSE CMAKE_FILES_C "cmake/*.cmake")
    list(
        FILTER
        CMAKE_FILES_TXT
        EXCLUDE
        REGEX
        "${CMAKE_SOURCE_DIR}/(build|external)/.*")
    set(CMAKE_FILES ${ROOT_CMAKE_FILES} ${CMAKE_FILES_TXT} ${CMAKE_FILES_C})
    find_program(CMAKE_FORMAT cmake-format)
    if(CMAKE_FORMAT)
        message(STATUS "Added Cmake Format")
        set(FORMATTING_COMMANDS)
        foreach(cmake_file ${CMAKE_FILES})
            list(
                APPEND
                FORMATTING_COMMANDS
                COMMAND
                cmake-format
                -c
                ${CMAKE_SOURCE_DIR}/.cmake-format
                -i
                ${cmake_file})
        endforeach()
        add_custom_target(
            run_cmake_format
            COMMAND ${FORMATTING_COMMANDS}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
    else()
        message(WARNING "CMAKE_FORMAT NOT FOUND")
    endif()
endfunction()

function(add_clang_format_target)
    if(NOT ${ENABLE_CLANG_FORMAT})
        return()
    endif()
    find_package(Python3 COMPONENTS Interpreter)
    if(NOT ${Python_FOUND})
        return()
    endif()
    file(GLOB_RECURSE CMAKE_FILES_CC "*/*.cc")
    file(GLOB_RECURSE CMAKE_FILES_CPP "*/*.cpp")
    file(GLOB_RECURSE CMAKE_FILES_H "*/*.h")
    file(GLOB_RECURSE CMAKE_FILES_HPP "*/*.hpp")
    set(CPP_FILES
        ${CMAKE_FILES_CC}
        ${CMAKE_FILES_CPP}
        ${CMAKE_FILES_H}
        ${CMAKE_FILES_HPP})
    list(
        FILTER
        CPP_FILES
        EXCLUDE
        REGEX
        "${CMAKE_SOURCE_DIR}/(build|external)/.*")
    find_program(CLANGFORMAT clang-format)
    if(CLANGFORMAT)
        message(STATUS "Added Clang Format")
        add_custom_target(
            run_clang_format
            COMMAND
                ${Python3_EXECUTABLE}
                ${CMAKE_SOURCE_DIR}/tools/run-clang-format.py ${CPP_FILES}
                --in-place
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            USES_TERMINAL)
    else()
        message(WARNING "CLANGFORMAT NOT FOUND")
    endif()
endfunction()

function(add_clang_tidy_to_target target)
    get_target_property(TARGET_SOURCES ${target} SOURCES)
    list(
        FILTER
        TARGET_SOURCES
        INCLUDE
        REGEX
        ".*.(cc|h|cpp|hpp)")

    find_package(Python3 COMPONENTS Interpreter)
    if(NOT ${Python_FOUND})
        message(WARNING "Python3 needed for Clang-Tidy")
        return()
    endif()

    find_program(CLANGTIDY clang-tidy)
    if(CLANGTIDY)
        if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
            message(STATUS "Added MSVC ClangTidy (VS GUI only) for: ${target}")
            set_target_properties(
                ${target} PROPERTIES VS_GLOBAL_EnableMicrosoftCodeAnalysis
                                     false)
            set_target_properties(
                ${target} PROPERTIES VS_GLOBAL_EnableClangTidyCodeAnalysis true)
        else()
            message(STATUS "Added Clang Tidy for Target: ${target}")
            add_custom_target(
                ${target}_clangtidy
                COMMAND
                    ${Python3_EXECUTABLE}
                    ${CMAKE_SOURCE_DIR}/tools/run-clang-tidy.py
                    ${TARGET_SOURCES}
                    -config-file=${CMAKE_SOURCE_DIR}/.clang-tidy
                    -extra-arg-before=-std=${CMAKE_CXX_STANDARD}
                    -header-filter="\(src|app\)\/*.\(h|hpp\)"
                    -p=${CMAKE_BINARY_DIR}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                USES_TERMINAL)
        endif()
    else()
        message(WARNING "CLANGTIDY NOT FOUND")
    endif()
endfunction()

#
# Function to print detailed project configuration and paths
#
function(print_project_configuration)
    message(STATUS "################################################################################")
    message(STATUS "#                            Project Configuration                             ")
    message(STATUS "################################################################################")

    # CMake version and generator
    message(STATUS "CMake version: ${CMAKE_VERSION}")
    message(STATUS "CMake generator: ${CMAKE_GENERATOR}")
    if (CMAKE_GENERATOR_PLATFORM)
        message(STATUS "CMake generator platform: ${CMAKE_GENERATOR_PLATFORM}")
    endif()
    if (CMAKE_GENERATOR_TOOLSET)
        message(STATUS "CMake generator toolset: ${CMAKE_GENERATOR_TOOLSET}")
    endif()

    # Project paths
    message(STATUS "Project source directory (CMAKE_SOURCE_DIR): ${CMAKE_SOURCE_DIR}")
    message(STATUS "Project binary directory (CMAKE_BINARY_DIR): ${CMAKE_BINARY_DIR}")
    message(STATUS "Install prefix (CMAKE_INSTALL_PREFIX): ${CMAKE_INSTALL_PREFIX}")
    message(STATUS "CMake module path (CMAKE_MODULE_PATH): ${CMAKE_MODULE_PATH}")

    # Build directories
    message(STATUS "CMake current binary directory (CMAKE_CURRENT_BINARY_DIR): ${CMAKE_CURRENT_BINARY_DIR}")
    message(STATUS "CMake current source directory (CMAKE_CURRENT_SOURCE_DIR): ${CMAKE_CURRENT_SOURCE_DIR}")
    if (CMAKE_RUNTIME_OUTPUT_DIRECTORY)
        message(STATUS "Runtime output directory (CMAKE_RUNTIME_OUTPUT_DIRECTORY): ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
    endif()
    if (CMAKE_LIBRARY_OUTPUT_DIRECTORY)
        message(STATUS "Library output directory (CMAKE_LIBRARY_OUTPUT_DIRECTORY): ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
    endif()
    if (CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
        message(STATUS "Archive output directory (CMAKE_ARCHIVE_OUTPUT_DIRECTORY): ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
    endif()

    # Build type
    if (CMAKE_BUILD_TYPE)
        message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
    else()
        message(STATUS "Build type: Not specified (multi-configuration generator)")
    endif()

    # Compiler information
    message(STATUS "C++ compiler (CMAKE_CXX_COMPILER): ${CMAKE_CXX_COMPILER}")
    message(STATUS "C++ compiler version: ${CMAKE_CXX_COMPILER_VERSION}")
    message(STATUS "C++ compiler flags (CMAKE_CXX_FLAGS): ${CMAKE_CXX_FLAGS}")
    message(STATUS "C++ compiler flags (Debug): ${CMAKE_CXX_FLAGS_DEBUG}")
    message(STATUS "C++ compiler flags (Release): ${CMAKE_CXX_FLAGS_RELEASE}")
    message(STATUS "C++ standard (CMAKE_CXX_STANDARD): ${CMAKE_CXX_STANDARD}")

    # Install directories
    message(STATUS "Install binary directory (CMAKE_INSTALL_BINDIR): ${CMAKE_INSTALL_BINDIR}")
    message(STATUS "Install library directory (CMAKE_INSTALL_LIBDIR): ${CMAKE_INSTALL_LIBDIR}")
    message(STATUS "Install include directory (CMAKE_INSTALL_INCLUDEDIR): ${CMAKE_INSTALL_INCLUDEDIR}")
    message(STATUS "Install CMake package directory: ${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}")

    # Additional directories
    message(STATUS "CMake cache directory (CMAKE_CACHEFILE_DIR): ${CMAKE_CACHEFILE_DIR}")
    if (CMAKE_TEMP_DIRECTORY)
        message(STATUS "Temporary directory (CMAKE_TEMP_DIRECTORY): ${CMAKE_TEMP_DIRECTORY}")
    endif()

    # Additional custom project variables (example)
    if (DEFINED PROJECT_CUSTOM_PATH)
        message(STATUS "Custom project path: ${PROJECT_CUSTOM_PATH}")
    endif()

    message(STATUS "################################################################################")
endfunction()
