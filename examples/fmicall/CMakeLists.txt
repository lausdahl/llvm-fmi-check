cmake_minimum_required (VERSION 3.11)
project (test C CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-discard-value-names")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../tools/llvmutil/cmake")
message(STATUS ${CMAKE_MODULE_PATH})
include(LLVMIRUtil)
include(ExternalProject)

SET(BUILD_TOOLS OFF CACHE BOOL "Build tools in the src directory (zipcmp, zipmerge, ziptool)")
SET(BUILD_REGRESS OFF CACHE BOOL "Build regression tests")
SET(BUILD_EXAMPLES OFF CACHE BOOL "Build examples")
SET(BUILD_DOC OFF CACHE BOOL "Build documentation")
SET(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries")

set(ENABLE_GNUTLS OFF CACHE BOOL "")
set(ENABLE_MBEDTLS OFF CACHE BOOL "")
set(ENABLE_OPENSSL OFF CACHE BOOL "")
set(ENABLE_WINDOWS_CRYPTO OFF CACHE BOOL "")

set(BUILD_TOOLS OFF CACHE BOOL "")
set(BUILD_REGRESS OFF CACHE BOOL "")
set(BUILD_EXAMPLES OFF CACHE BOOL "")
set(BUILD_DOC OFF CACHE BOOL "")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "")

include_directories(${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/libs)

add_executable(test main.cpp)
target_link_libraries(test rt dl)

install(TARGETS test
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION bin)


# this property is required by our parasitic targets
set_target_properties(test PROPERTIES LINKER_LANGUAGE CXX)

llvmir_attach_bc_target(test_bc test)
add_dependencies(test test_bc)
llvmir_attach_disassemble_target(test_dis test_bc)
add_dependencies(test_dis test_bc)

