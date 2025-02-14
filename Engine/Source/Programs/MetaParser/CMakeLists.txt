find_package(kainjow_mustache REQUIRED)
include(${VOXIUM_ROOT_DIR}/Engine/Source/ThirdParty/LLVM.cmake)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB_RECURSE HEADERS "*.h")
file(GLOB_RECURSE SOURCES "*.cpp")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${HEADERS} ${SOURCES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Parser)

add_executable(${META_PARSER_TARGET} ${HEADERS} ${SOURCES})
target_link_libraries(
    ${META_PARSER_TARGET}
    PUBLIC
        ProjectOptions
        LLVM::LLVM
        kainjow_mustache::kainjow_mustache
)

set_target_properties(${META_PARSER_TARGET} PROPERTIES FOLDER "Programs")

# copy resources on post build
add_custom_command(TARGET ${META_PARSER_TARGET} POST_BUILD
	# mustache templates directory
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${LLVM_SHARED_LIBRARY_DIR}"
		$<TARGET_FILE_DIR:${META_PARSER_TARGET}>
)
