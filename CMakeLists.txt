cmake_minimum_required(VERSION 3.20)
project(RayTracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/cmake")

include(CPM)
include(CheckLanguage)
include(sfml)
include(json)
include(stl_reader)
include(openMP)

#### Set shader language to compile with
if (APPLE)
    set(USE_SHADER_METAL ON)
    set(CMAKE_XCODE_GENERATE_SCHEME TRUE)
endif ()
check_language(CUDA)
if(CMAKE_CUDA_COMPILER)
    set(USE_SHADER_CUDA ON)
    enable_language(CUDA)
endif()

# Alle .cpp-Dateien automatisch einbinden
file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(${PROJECT_NAME} ${SRC_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/)

## adding metal and cuda libraries
add_subdirectory(lib)
## adding respective shaders for compilation
add_subdirectory(shader)

# Get the latest commit hash
execute_process(
        COMMAND git rev-parse HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE)

add_definitions(-DPLATFORM_NAME="${CMAKE_SYSTEM_NAME}" -DARCHITECTURE="${CMAKE_HOST_SYSTEM_PROCESSOR}" -DGIT_COMMIT_HASH="${GIT_COMMIT_HASH}")

add_custom_target(CopyResources
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/cnr.otf ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/scene ${CMAKE_BINARY_DIR}/scene
)
add_dependencies(${PROJECT_NAME} CopyResources)

## include definitions header for the right platform
include_directories(definitions)
if (WIN32)
    if (MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /FI "cuda.hpp")
    else ()
        target_compile_options(${PROJECT_NAME} PRIVATE -include "cuda.hpp")
    endif ()
elseif (APPLE)
    target_compile_options(${PROJECT_NAME} PRIVATE -include "metal.hpp")
    set_property(TARGET ${PROJECT_NAME} PROPERTY XCODE_EMBED_FRAMEWORKS "Metal Foundation")
    set_property(TARGET ${PROJECT_NAME} PROPERTY XCODE_SCHEME_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
else()
    if(USE_SHADER_CUDA)
        target_compile_options(${PROJECT_NAME} PRIVATE -include "cuda.hpp")
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -include "definitions.hpp")
    endif ()
endif ()