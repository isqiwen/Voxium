set(VOXIUM_CODE_GEN_PARAMS_IN_PATH "${ENGINE_ROOT_DIR}/Source/Programs/CodeGen/CodeGen.txt.in")
set(VOXIUM_CODE_GEN_PARAMS_PATH "${CMAKE_BINARY_DIR}/CodeGen.txt")
configure_file(${VOXIUM_CODE_GEN_PARAMS_IN_PATH} ${VOXIUM_CODE_GEN_PARAMS_PATH})

#
# use wine for linux
if (CMAKE_HOST_WIN32)
    set(CODE_GEN_PRE_EXE)
    set(CODE_GEN_PARSER ${EXECUTABLE_DIR}/${META_PARSER_TARGET}.exe)
    set(sys_include "*")
elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux" )
    set(CODE_GEN_PRE_EXE)
    set(CODE_GEN_PARSER ${EXECUTABLE_DIR}/${META_PARSER_TARGET})
    set(sys_include "/usr/include/c++/9/")
    #execute_process(COMMAND chmod a+x ${CODE_GEN_PARSER} WORKING_DIRECTORY ${EXECUTABLE_DIR})
elseif(CMAKE_HOST_APPLE)
    find_program(XCRUN_EXECUTABLE xcrun)
    if(NOT XCRUN_EXECUTABLE)
      message(FATAL_ERROR "xcrun not found!!!")
    endif()

    execute_process(
      COMMAND ${XCRUN_EXECUTABLE} --sdk macosx --show-sdk-platform-path
      OUTPUT_VARIABLE osx_sdk_platform_path_test
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    set(CODE_GEN_PRE_EXE)
    set(CODE_GEN_PARSER ${EXECUTABLE_DIR}/${META_PARSER_TARGET})
    set(sys_include "${osx_sdk_platform_path_test}/../../Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1")
endif()

set (PARSER_INPUT ${CMAKE_BINARY_DIR}/parser_header.h)
### BUILDING ====================================================================================

# Called first time when building target
add_custom_target(${CODE_GEN_TARGET} ALL
    # If more than one COMMAND is specified they will be executed in order...
    COMMAND
        ${CMAKE_COMMAND} -E echo "************************************************************* "
    COMMAND
        ${CMAKE_COMMAND} -E echo "**** [CODE_GEN] BEGIN "
    COMMAND
        ${CMAKE_COMMAND} -E echo "************************************************************* "

    COMMAND
        ${CODE_GEN_PARSER} "${VOXIUM_CODE_GEN_PARAMS_PATH}"  "${PARSER_INPUT}"  "${ENGINE_ROOT_DIR}/Source/Runtime" ${sys_include} "Voxium" 0
    ### BUILDING ====================================================================================
    COMMAND
        ${CMAKE_COMMAND} -E echo "+++ CODE_GEN finished +++"
)
