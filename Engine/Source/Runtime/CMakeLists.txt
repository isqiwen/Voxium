find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)
find_package(glm REQUIRED)
find_package(ZLIB REQUIRED)
find_package(glfw3 REQUIRED)

include(${THIRD_PARTY_DIR}/Vulkan.cmake)

file(GLOB_RECURSE HEADER_FILES "Core/*.h")
file(GLOB_RECURSE SOURCE_FILES "Core/*.cpp")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${HEADER_FILES} ${SOURCE_FILES})

add_library(${RUNTIME_TARGET} SHARED ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(
    ${RUNTIME_TARGET}
    PRIVATE
        ProjectOptions
        nlohmann_json::nlohmann_json
        spdlog::spdlog_header_only
        glm::glm
        ZLIB::ZLIB
        glfw
        Vulkan::Vulkan
)

target_compile_definitions(${RUNTIME_TARGET} PRIVATE CORE_EXPORTS)

set_target_properties(${RUNTIME_TARGET} PROPERTIES FOLDER "Engine")

target_include_directories(
  ${RUNTIME_TARGET}
  PUBLIC $<BUILD_INTERFACE:${ENGINE_ROOT_DIR}/Source/Runtime/Core/Public>
  $<INSTALL_INTERFACE:include/${RUNTIME_TARGET}-${PROJECT_VERSION}>
)

target_include_directories(
  ${RUNTIME_TARGET}
  PUBLIC $<BUILD_INTERFACE:${ENGINE_ROOT_DIR}/Source/Runtime/Core>
  $<INSTALL_INTERFACE:include/${RUNTIME_TARGET}-${PROJECT_VERSION}>
)

target_include_directories(
  ${RUNTIME_TARGET}
  PUBLIC $<BUILD_INTERFACE:${ENGINE_ROOT_DIR}/Source/Runtime>
  $<INSTALL_INTERFACE:include/${RUNTIME_TARGET}-${PROJECT_VERSION}>
)

### precompile
# set global var used by precompile
set(VOXIUM_RUNTIME_HEADS "${HEADER_FILES}" PARENT_SCOPE)

if(${PROJECT_NAME}_ENABLE_UNIT_TESTING)
    enable_testing()
    add_subdirectory(Test)
endif()
