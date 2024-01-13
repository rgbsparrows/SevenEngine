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
add_compile_options("/GR-")
add_compile_options("/MP")
add_compile_options("/WX")
add_compile_options("/Zc:char8_t-")
add_compile_definitions("$<$<CONFIG:Debug>:SE_BUILD_DEBUG>$<$<CONFIG:Shipping>:SE_BUILD_SHIPPING>$<$<CONFIG:Profile>:SE_BUILD_PROFILE>$<$<CONFIG:Test>:SE_BUILD_TEST>=1")
add_compile_definitions("SE_BUILD_CONFIG=$<CONFIGURATION>")
add_compile_definitions("FBXSDK_DEFINE_NAMESPACE=1")
add_compile_definitions("FBXSDK_NAMESPACE_USING=0")

include(../Sdk/SdkList.txt)

set(ENGINE_FILES)
set(ENGINE_INCLUDE)
set(ENGINE_DEBUG_LIB)
set(ENGINE_RELEASE_LIB)
set(ENGINE_DEBUG_DLL)
set(ENGINE_RELEASE_DLL)

file(GLOB_RECURSE srcFile ${ENGINE_DIR}/Source/*)
file(GLOB_RECURSE shaderFile ${ENGINE_DIR}/Shaders/*)
file(GLOB_RECURSE configFile ${ENGINE_DIR}/Config/*)

list(APPEND ENGINE_FILES ${srcFile} ${shaderFile} ${configFile})

file(GLOB_RECURSE srcDir LIST_DIRECTORIES true ${ENGINE_DIR}/Source/*/)
foreach(includeDir ${srcDir})
    get_filename_component(name ${includeDir} NAME)
#将Private目录添加到include路径主要是不想在模块中的private实现中，出现include "../XXX"之类的情况，但不要在其他的模块中引用private目录，Include时应该始终Include以模块名称为路径头的路径
    if(EXISTS ${includeDir}/${name} AND EXISTS ${includeDir}/Private)
        list(APPEND ENGINE_INCLUDE ${includeDir})
        list(APPEND ENGINE_INCLUDE ${includeDir}/Private)
    endif()
endforeach(includeDir)

foreach(sdk ${ENGINE_SDKS})
    if (DEFINED ${sdk}_INCLUDE)
        file(GLOB_RECURSE sdkIncludeFiles ${${sdk}_INCLUDE}/*)
        list(APPEND ENGINE_FILES ${sdkIncludeFiles})
        list(APPEND ENGINE_INCLUDE ${${sdk}_INCLUDE})
    endif()

    if (DEFINED ${sdk}_SOURCE)
        file(GLOB_RECURSE sdkSourceFiles ${${sdk}_SOURCE}/*)
        list(APPEND ENGINE_FILES ${sdkSourceFiles})
    endif()

    if (DEFINED ${sdk}_DEBUGLIB)
        list(APPEND ENGINE_DEBUG_LIB ${${sdk}_DEBUGLIB})
    endif()
    
    if (DEFINED ${sdk}_RELEASELIB)
        list(APPEND ENGINE_RELEASE_LIB ${${sdk}_RELEASELIB})
    endif()

    if (DEFINED ${sdk}_DEBUGDLL)
        list(APPEND ENGINE_DEBUG_DLL ${${sdk}_DEBUGDLL})
    endif()
    
    if (DEFINED ${sdk}_RELEASEDLL)
        list(APPEND ENGINE_RELEASE_DLL ${${sdk}_RELEASEDLL})
    endif()

endforeach(sdk)

add_executable(SevenEngine ${ENGINE_FILES})
include_directories(SevenEngine ${ENGINE_INCLUDE})

target_link_directories(SevenEngine PRIVATE $<$<CONFIG:Debug>:${ENGINE_DEBUG_LIB}>$<$<CONFIG:Shipping>:${ENGINE_RELEASE_LIB}>$<$<CONFIG:Profile>:${ENGINE_RELEASE_LIB}>$<$<CONFIG:Test>:${ENGINE_RELEASE_LIB}>)

foreach(dllDir $<$<CONFIG:Debug>:${ENGINE_DEBUG_DLL}>$<$<CONFIG:Shipping>:${ENGINE_RELEASE_DLL}>$<$<CONFIG:Profile>:${ENGINE_RELEASE_DLL}>$<$<CONFIG:Test>:${ENGINE_RELEASE_DLL}>)
    add_custom_command(TARGET SevenEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${dllDir}
            $<TARGET_FILE_DIR:SevenEngine>
    )
endforeach(dllDir)

source_group(TREE ${ENGINE_DIR}/ FILES ${ENGINE_FILES} )

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
SET_PROPERTY(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "_CmakeProject")
SET_PROPERTY(TARGET SevenEngine PROPERTY FOLDER "Engine")

file(WRITE ${ENGINE_DIR}/Intermediate/SevenEngine.vcxproj.user "<?xml version=\"1.0\" encoding=\"utf-8\"?>
<Project ToolsVersion=\"Current\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">
  <PropertyGroup>
    <LocalDebuggerCommandArguments>EnginePath=\"$(SolutionDir)..\"</LocalDebuggerCommandArguments>
    <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
  </PropertyGroup>
  <PropertyGroup>
    <LocalDebuggerWorkingDirectory>$(ProjectDir)..\\Binaries\\$(Configuration)</LocalDebuggerWorkingDirectory>
  </PropertyGroup>
</Project>")
