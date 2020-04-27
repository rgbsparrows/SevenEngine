cmake_minimum_required(VERSION 3.6)

project(SevenEngine CXX)

# check
if(NOT MSVC)
    message(FATAL_ERROR "需要使用vs进行编译")
endif()

if(MSVC_VERSION LESS 1920)
    message(FATAL_ERROR "vs版本过低(至少应使用vs2019版本)")
endif()

# set config
set(CMAKE_CONFIGURATION_TYPES Debug Shipping Profile Test)
set(ENGINE_DIR ${PROJECT_SOURCE_DIR}/..)
set(ENGINE_SDK_DIR ${ENGINE_DIR}/Sdk)
set(CMAKE_INSTALL_PREFIX ${ENGINE_DIR})
set(CMAKE_EXE_LINKER_FLAGS_TEST ${CMAKE_EXE_LINKER_FLAGS_RELEASE})
set(CMAKE_EXE_LINKER_FLAGS_PROFILE ${CMAKE_EXE_LINKER_FLAGS_RELEASE})
set(CMAKE_EXE_LINKER_FLAGS_SHIPPING ${CMAKE_EXE_LINKER_FLAGS_RELEASE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ENGINE_DIR}/Binaries/${CMAKE_BUILD_TYPE})
set(CMAKE_WIN32_EXECUTABLE ON)

add_compile_options("/std:c++latest")
add_compile_definitions("$<$<CONFIG:Debug>:SE_BUILD_DEBUG>$<$<CONFIG:Shipping>:SE_BUILD_SHIPPING>$<$<CONFIG:Profile>:SE_BUILD_PROFILE>$<$<CONFIG:Test>:SE_BUILD_TEST>=1")
add_compile_definitions("SE_BUILD_CONFIG=$<CONFIGURATION>")

include(SdkList.txt)

set(ENGINE_FILES)
set(ENGINE_INCLUDE)
set(ENGINE_DEBUG_LIB)
set(ENGINE_RELEASE_LIB)

file(GLOB_RECURSE srcFile ${ENGINE_DIR}/Source/*)
file(GLOB_RECURSE shaderFile ${ENGINE_DIR}/Shaders/*)
file(GLOB_RECURSE configFile ${ENGINE_DIR}/Config/*)

list(APPEND ENGINE_FILES ${srcFile} ${shaderFile} ${configFile})

file(GLOB_RECURSE srcDir LIST_DIRECTORIES true ${ENGINE_DIR}/Source/Engine/*/)
foreach(includeDir ${srcDir})
    get_filename_component(name ${includeDir} NAME)
    if(${name} STREQUAL "Public")
        list(APPEND ENGINE_INCLUDE ${includeDir})
    endif()
endforeach(includeDir)

foreach(sdk IN ${ENGINE_SDKS})
    file(GLOB_RECURSE sdkFiles ${${sdk}_INCLUDE})
    list(APPEND ENGINE_FILES ${sdkFiles})
    list(APPEND ENGINE_INCLUDE ${sdkFiles})
    list(APPEND ENGINE_DEBUG_LIB ${${sdk}_DEBUGLIB})
    list(APPEND ENGINE_RELEASE_LIB ${${sdk}_RELEASELIB})
endforeach()

add_executable(SevenEngine ${ENGINE_FILES})
include_directories(SevenEngine ${ENGINE_INCLUDE})

target_link_libraries(SevenEngine $<$<CONFIG:Debug>:${ENGINE_DEBUG_LIB}>$<$<CONFIG:Shipping>:${ENGINE_RELEASE_LIB}>$<$<CONFIG:Profile>:${ENGINE_RELEASE_LIB}>$<$<CONFIG:Test>:${ENGINE_RELEASE_LIB}>)

source_group(TREE ${ENGINE_DIR}/ FILES ${ENGINE_FILES} )

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
SET_PROPERTY(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "_CmakeProject")
SET_PROPERTY(TARGET SevenEngine PROPERTY FOLDER "Engine")
