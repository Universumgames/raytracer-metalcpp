file(GLOB SHADER_SRC *.metal)
file(GLOB SHADER_HEADERS *.h*)

set(METALLIB ${CMAKE_CURRENT_BINARY_DIR}/default.metallib)

add_custom_command(
        OUTPUT ${METALLIB}
        COMMAND xcrun -sdk macosx metal -frecord-sources=flat ${SHADER_SRC}
        DEPENDS ${SHADER_SRC}
        VERBATIM
)

# Tell CMake the metallib is generated so IDEs show it correctly
set_source_files_properties(${METALLIB} PROPERTIES GENERATED TRUE)

# Create an object library that contains the shader sources and the generated metallib
add_library(METAL_SHADERS OBJECT ${SHADER_SRC} ${SHADER_HEADERS} ${METALLIB})

set_target_properties(METAL_SHADERS PROPERTIES
        DEBUG_POSTFIX ""
        XCODE_PRODUCT_TYPE com.apple.product-type.metal-library
        XCODE_ATTRIBUTE_MTL_FAST_MATH "YES"
        XCODE_ATTRIBUTE_MTL_ENABLE_DEBUG_INFO[variant=Debug] "INCLUDE_SOURCE"
        XCODE_ATTRIBUTE_MTL_ENABLE_DEBUG_INFO[variant=RelWithDebInfo] "INCLUDE_SOURCE"
        XCODE_ATTRIBUTE_MTL_HEADER_SEARCH_PATHS "$(HEADER_SEARCH_PATHS)"
        LANGUAGE Metal
        LINKER_LANGUAGE Metal
)

target_sources(${PROJECT_NAME} PRIVATE ${SHADER_SRC} ${SHADER_HEADERS} ${METALLIB})

add_custom_target(
        COPIED_METAL_SHADERS
        COMMAND ${CMAKE_COMMAND} -E copy ${METALLIB} ${CMAKE_CURRENT_BINARY_DIR}/../../
        DEPENDS METAL_SHADERS
)

add_dependencies(COPIED_METAL_SHADERS METAL_SHADERS)