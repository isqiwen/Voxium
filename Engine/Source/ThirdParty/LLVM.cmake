include_guard(GLOBAL)

set(LLVM_ROOT_DIR_ "${VOXIUM_ROOT_DIR}/Engine/Source/ThirdParty/LLVM")
set(LLVM_INCLUDE_DIR "${LLVM_ROOT_DIR_}/include")

if (CMAKE_HOST_WIN32)
    set(LLVM_SHARED_LIBRARY_DIR "${LLVM_ROOT_DIR_}/bin/x64")
    set(LLVM_SHARED_LIBRARY "${LLVM_ROOT_DIR_}/bin/x64/libclang.dll")
    set(LLVM_IMPORT_LIBRARY "${LLVM_ROOT_DIR_}/lib/x64/libclang.lib")
elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
    set(LLVM_SHARED_LIBRARY_DIR "${LLVM_ROOT_DIR_}/bin/Linux")
    set(LLVM_SHARED_LIBRARY "${LLVM_ROOT_DIR_}/bin/Linux/libclang.so.12")
    set(LLVM_IMPORT_LIBRARY "${LLVM_ROOT_DIR_}/lib/Linux/libclang.so.12")
elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")  # macOS
    set(LLVM_SHARED_LIBRARY_DIR "${LLVM_ROOT_DIR_}/bin/macOS")
    set(LLVM_SHARED_LIBRARY "${LLVM_ROOT_DIR_}/bin/macOS/libclang.dylib")
    set(LLVM_IMPORT_LIBRARY "${LLVM_ROOT_DIR_}/lib/macOS/libclang.dylib")
else()
    message(FATAL_ERROR "Unsupported OS for LLVM configuration!")
endif()

add_library(LLVM::LLVM SHARED IMPORTED)

set_target_properties(LLVM::LLVM PROPERTIES
    IMPORTED_LOCATION "${LLVM_SHARED_LIBRARY}"
    IMPORTED_IMPLIB "${LLVM_IMPORT_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${LLVM_INCLUDE_DIR}"
)
