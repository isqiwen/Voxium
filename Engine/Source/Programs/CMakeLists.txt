set(META_PARSER_TARGET "VoxiumMetaParser")
set(CODE_GEN_TARGET "VoxiumCodeGen")

add_subdirectory(MetaParser)

include(CodeGen/CodeGen.cmake)
set_target_properties(${CODE_GEN_TARGET} PROPERTIES FOLDER "Engine")

# add_dependencies(${RUNTIME_TARGET} ${CODE_GEN_TARGET})
add_dependencies(${CODE_GEN_TARGET} ${META_PARSER_TARGET})
