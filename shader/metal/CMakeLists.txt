file(GLOB SHADER_SRC *.metal)
file(GLOB SHADER_HEADERS *.h*)

add_custom_command(
        OUTPUT default.metallib
        COMMAND xcrun -sdk macosx metal -frecord-sources=flat ${SHADER_SRC}
        DEPENDS ${SHADER_SRC}
        VERBATIM
)

add_library(METAL_SHADERS OBJECT)
target_sources(METAL_SHADERS PRIVATE default.metallib ${SHADER_SRC} ${SHADER_HEADERS})

add_custom_target(
        COPIED_METAL_SHADERS
        COMMAND cp default.metallib ../../
        COMMAND cp default.metallibsym ../../)

add_dependencies(COPIED_METAL_SHADERS METAL_SHADERS)