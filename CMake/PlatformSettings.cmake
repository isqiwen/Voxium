# Function to apply platform-specific compiler and linker settings to a target
function(set_platform_general_settings target_name)
    # Check if the platform is Windows
    if (WIN32)
        # Enable symbol export for shared libraries
        # Ensures that all symbols are automatically exported in shared libraries
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

        # Add Windows-specific macro definitions
        target_compile_definitions(${target_name} INTERFACE
            WIN32                      # Define the WIN32 macro for compatibility with Windows APIs
            _WIN32                     # Define the _WIN32 macro for Windows-specific code
            _WINDOWS                   # Define the _WINDOWS macro for Windows-specific logic
            NOMINMAX                   # Prevent Windows headers from defining min() and max() macros
            -DUNICODE
            -D_UNICODE
        )

        # Add 64-bit system definitions for Windows
        if (CMAKE_SIZEOF_VOID_P EQUAL 8)  # Check if the system is 64-bit
            target_compile_definitions(${target_name} INTERFACE
                WIN64                  # Define the WIN64 macro for compatibility with 64-bit Windows APIs
                _WIN64                 # Define the _WIN64 macro for 64-bit Windows-specific code
            )
        endif ()

        # Add Windows-specific linker options
        target_link_options(${target_name} INTERFACE
            "/INCREMENTAL:NO"                            # Disable incremental linking globally
            $<$<CONFIG:Debug>:/DEBUG>       # Enable debug symbols in Debug configuration
            $<$<CONFIG:Debug>:/INCREMENTAL:NO> # Disable incremental linking in Debug configuration
        )

        if (MSVC)
            set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

            target_compile_options(${target_name} INTERFACE
                $<$<CONFIG:Release>:/W4 /MD /O2>
                $<$<CONFIG:Debug>:/W4 /MDd>
            )

            set(CMAKE_CXX_FLAGS
                "${CMAKE_CXX_FLAGS} /source-charset:UTF-8 /D_DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR"
            )
        else()
            if (CMAKE_BUILD_TYPE STREQUAL "Debug")
                target_compile_options(${target_name} INTERFACE -Og)
            endif()
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
        endif()

    # Check if the platform is macOS
    elseif (APPLE)
        # Enable position-independent code (PIC) for macOS
        # PIC is required for shared libraries to enable runtime relocation of addresses
        set(CMAKE_POSITION_INDEPENDENT_CODE ON)

    # For other platforms (e.g., Linux)
    else ()
        # Enable position-independent code (PIC) for Linux or other platforms
        # Ensures compatibility with shared libraries and dynamic linking
        set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    endif ()
endfunction()
