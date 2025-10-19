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