cmake_minimum_required(VERSION 3.20)
project(RayTracer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#### Set shader language to compile with
set(USE_SHADER_METAL ON)
#set(USER_SHADER_CUDA OFF)

include(cmake/CMakeLists.txt)

# Alle .cpp-Dateien automatisch einbinden
file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(${PROJECT_NAME} ${SRC_FILES})

## adding metal and cuda libraries
add_subdirectory(lib)
## adding respective shaders for compilation
add_subdirectory(shader)

add_definitions(-DPLATFORM_NAME="${CMAKE_SYSTEM_NAME}" -DARCHITECTURE="${CMAKE_HOST_SYSTEM_PROCESSOR}")


add_custom_target(CopyResources
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/cnr.otf ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/scene ${CMAKE_BINARY_DIR}/scene
)
add_dependencies(${PROJECT_NAME} CopyResources)

## include definitions header for the right platform
include_directories(definitions)
if(WIN32)
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /FI"cuda.hpp")
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -include "cuda.hpp")
    endif()
elseif (APPLE)
    target_compile_options(${PROJECT_NAME} PRIVATE -include "metal.hpp")
endif ()