set(ENGINE_ROOT_DIR "${VOXIUM_ROOT_DIR}/Engine")

set(RUNTIME_TARGET "VoxiumRuntime")

add_subdirectory(Source/Runtime)
add_subdirectory(Source/Programs)

# set(CODEGEN_TARGET "VoxiumPreCompile")
# include(Source/precompile/precompile.cmake)
# set_target_properties("${CODEGEN_TARGET}" PROPERTIES FOLDER "Engine" )


# add_dependencies(${RUNTIME_TARGET} "${CODEGEN_TARGET}")
# add_dependencies("${CODEGEN_TARGET}" "VoxiumParser")
