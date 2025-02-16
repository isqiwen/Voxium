include_guard(GLOBAL)

set(VULKAN_SDK_PATH "${THIRD_PARTY_DIR}/VulkanSDK")

set(VULKAN_INCLUDE_DIRS "${VULKAN_SDK_PATH}/include")

if(WIN32)
    set(VULKAN_LIBRARY "${VULKAN_SDK_PATH}/lib/Win32/vulkan-1.lib")
    set(GLSLANG_VALIDATOR_EXECUTABLE "${VULKAN_SDK_PATH}/bin/Win32/glslangValidator.exe")
    add_compile_definitions("PICCOLO_VK_LAYER_PATH=${VULKAN_SDK_PATH}/bin/Win32")
elseif(APPLE)
    set(VULKAN_LIBRARY "${VULKAN_SDK_PATH}/lib/MacOS/libvulkan.1.dylib")
    set(GLSLANG_VALIDATOR_EXECUTABLE "${VULKAN_SDK_PATH}/bin/MacOS/glslangValidator")
    add_compile_definitions("PICCOLO_VK_LAYER_PATH=${VULKAN_SDK_PATH}/bin/MacOS")
    add_compile_definitions("PICCOLO_VK_ICD_FILENAMES=${VULKAN_SDK_PATH}/bin/MacOS/MoltenVK_icd.json")
elseif(UNIX)
    set(VULKAN_LIBRARY "${VULKAN_SDK_PATH}/lib/Linux/libvulkan.so.1")
    set(GLSLANG_VALIDATOR_EXECUTABLE "${VULKAN_SDK_PATH}/bin/Linux/glslangValidator")
    add_compile_definitions("PICCOLO_VK_LAYER_PATH=${VULKAN_SDK_PATH}/bin/Linux")
else()
    message(FATAL_ERROR "Unknown platform: Vulkan SDK path not set.")
endif()

set(Vulkan_LIBRARIES ${VULKAN_LIBRARY})

add_library(Vulkan::Vulkan UNKNOWN IMPORTED)
set_target_properties(Vulkan::Vulkan PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${Vulkan_INCLUDE_DIRS}"
    IMPORTED_LOCATION "${Vulkan_LIBRARIES}"
)

set(GLSLANG_VALIDATOR ${GLSLANG_VALIDATOR_EXECUTABLE} CACHE STRING "Path to glslangValidator")

message(STATUS "Vulkan SDK found at: ${VULKAN_SDK_PATH}")
message(STATUS "Vulkan Include: ${Vulkan_INCLUDE_DIRS}")
message(STATUS "Vulkan Library: ${Vulkan_LIBRARIES}")
message(STATUS "GLSL Compiler: ${GLSLANG_VALIDATOR_EXECUTABLE}")
