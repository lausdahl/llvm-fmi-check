cmake_minimum_required (VERSION 3.11)
project (sim C CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../tools/llvmutil/cmake")
message(STATUS ${CMAKE_MODULE_PATH})
include(LLVMIRUtil)


include(FetchContent)
include(ExternalProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-discard-value-names")

FetchContent_Declare(intocpsfmi
  GIT_REPOSITORY https://github.com/INTO-CPS-Association/org.intocps.maestro.fmi.git
        GIT_TAG  Release/1.0.12
        GIT_SHALLOW ON
 SOURCE_DIR "${CMAKE_BINARY_DIR}/intocpsfmi-src"
)

FetchContent_Populate(intocpsfmi)
set(intocpsfmi-src "intocpsfmi-src")


SET(BUILD_TOOLS OFF CACHE BOOL "Build tools in the src directory (zipcmp, zipmerge, ziptool)")
SET(BUILD_REGRESS OFF CACHE BOOL "Build regression tests")
SET(BUILD_EXAMPLES OFF CACHE BOOL "Build examples")
SET(BUILD_DOC OFF CACHE BOOL "Build documentation")
SET(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries")

FetchContent_Declare(libzip
        GIT_REPOSITORY https://github.com/nih-at/libzip.git
        GIT_TAG  v1.7.3
        GIT_SHALLOW ON

        SOURCE_DIR "${CMAKE_BINARY_DIR}/libzip"
)


set(ENABLE_GNUTLS OFF CACHE BOOL "")
set(ENABLE_MBEDTLS OFF CACHE BOOL "")
set(ENABLE_OPENSSL OFF CACHE BOOL "")
set(ENABLE_WINDOWS_CRYPTO OFF CACHE BOOL "")

set(BUILD_TOOLS OFF CACHE BOOL "")
set(BUILD_REGRESS OFF CACHE BOOL "")
set(BUILD_EXAMPLES OFF CACHE BOOL "")
set(BUILD_DOC OFF CACHE BOOL "")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "")

FetchContent_MakeAvailable(libzip )

SET(RAPIDJSON_BUILD_EXAMPLES OFF CACHE BOOL "Build rapidjson examples" FORCE)
SET(RAPIDJSON_BUILD_TESTS OFF CACHE BOOL "Build rapidjson perftests and unittests" FORCE)
FetchContent_Declare(rapidjson
        GIT_REPOSITORY https://github.com/Tencent/rapidjson.git
        GIT_TAG v1.1.0
     #   GIT_SHALLOW ON
        SOURCE_DIR "${CMAKE_BINARY_DIR}/rapidjson"
        BUILD_COMMAND ""
        CONFIGURE_COMMAND ""
        INSTALL_COMMAND ""
        TEST_COMMAND ""
        )

FetchContent_Populate(rapidjson)

#file(CHMOD_RECURSE "${CMAKE_BINARY_DIR}/rapidjson" PERMISSIONS OWNER_READ OWNER_WRITE  GROUP_READ GROUP_WRITE WORLD_READ WORLD_WRITE )

include_directories(${PROJECT_SOURCE_DIR}
    ${intocpsfmi_SOURCE_DIR}/jnifmuapi/src/main/native/src/external/shared/fmi/include
    ${intocpsfmi_SOURCE_DIR}/jnifmuapi/src/main/native/src/external/shared
    ${intocpsfmi_SOURCE_DIR}/jnifmuapi/src/main/native/src
    ${PROJECT_SOURCE_DIR}/libs
    ${CMAKE_BINARY_DIR}/rapidjson/include)



add_library(sim_c STATIC
    ${CMAKE_BINARY_DIR}/intocpsfmi-src/jnifmuapi/src/main/native/src/sim_support.c
    ${PROJECT_SOURCE_DIR}/libs/uri.c
)

add_executable(sim
 #   ${intocpsfmi-src}/jnifmuapi/src/main/native/src/sim_support.c
 #   libs/uri.c
    libs/DataWriter.cpp
    libs/BooleanLogic.cpp
    libs/Logger.cpp
    libs/SimFmi2.cpp
    libs/SimMath.cpp
    libs/MEnv.cpp
    libs/unzip.cpp
    libs/VariableStep.cpp
    libs/SimulationControl.cpp
    co-sim.cxx
    main.cpp)

add_custom_target(copy)
add_custom_command(TARGET copy PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E
        copy ${PROJECT_SOURCE_DIR}/fmi2_annotate.h ${CMAKE_BINARY_DIR}/intocpsfmi-src/jnifmuapi/src/main/native/src/external/shared/fmi2.h
)
add_dependencies(sim_c copy)

target_link_libraries(sim zip sim_c)
target_compile_definitions(sim PRIVATE -DFMI_COSIMULATION)

if (UNIX)
	if(APPLE)

	else()
		target_link_libraries(sim rt dl)
	endif()
endif()


install(TARGETS sim
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION bin)


#set_target_properties(sim PROPERTIES
#  CXX_STANDARD 17
#  CXX_EXTENSIONS OFF)


# this property is required by our parasitic targets
set_target_properties(sim_c PROPERTIES LINKER_LANGUAGE C)
set_target_properties(sim PROPERTIES LINKER_LANGUAGE CXX)
#llvmir_attach_bc_target(sim_bc sim)
#target_compile_options(sim PUBLIC "-O0")
#llvmir_attach_opt_pass_target(sim_pass1 sim_bc -mem2reg)


set_target_properties(sim PROPERTIES COMPILE_OPTIONS "-isysroot;/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk")
set_target_properties(sim_c PROPERTIES COMPILE_OPTIONS "-isysroot;/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk")


target_include_directories(sim PRIVATE ${CMAKE_BINARY_DIR}/libzip/lib -I ${CMAKE_BINARY_DIR}/_deps/libzip-build )

llvmir_attach_bc_target(sim_bc sim)
add_dependencies(sim sim_bc)
llvmir_attach_disassemble_target(sim_dis sim_bc)
add_dependencies(sim_dis sim_bc)


llvmir_attach_bc_target(sim_c_bc sim_c)
add_dependencies(sim_c sim_c_bc)
llvmir_attach_disassemble_target(sim_c_dis sim_c_bc)
add_dependencies(sim_c_dis sim_c_bc)


#llvmir_attach_opt_pass_target(sim_pass1 sim_bc -mem2reg)
#llvmir_attach_opt_pass_target(sim_pass2 sim_pass1 -simplifycfg -licm)
#llvmir_attach_opt_pass_target(sim_pass3 sim_pass2 -licm)

#llvmir_attach_disassemble_target(sim_dis sim_pass2)
#llvmir_attach_assemble_target(sim_as sim_dis)

#llvmir_attach_link_target(sim_llvmlink sim_pass2)

#llvmir_attach_opt_pass_target(sim_pass4 sim_llvmlink -simplifycfg)

#llvmir_attach_library(sim_bc_lib sim_pass2 SHARED)

#add_dependencies(sim sim_bc )
#add_dependencies(sim_c sim_c_bc )
