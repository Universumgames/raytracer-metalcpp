file(GLOB SHADER_SRC *.metal)

set(SHADER_INTERMED_DIR ${CMAKE_CURRENT_BINARY_DIR}/metal_ir)

file(MAKE_DIRECTORY ${SHADER_INTERMED_DIR})

# Compile Shaders to intermediates
foreach(SRC ${SHADER_SRC})
    get_filename_component(SHADER_INTERMED_TARGET ${SRC} NAME_WLE)
    set(SHADER_IR_DESTINATION ${SHADER_INTERMED_DIR}/${SHADER_INTERMED_TARGET}.ir)
    add_custom_target(METAL_${SHADER_INTERMED_TARGET}_IR
        COMMAND xcrun -sdk macosx metal -o ${SHADER_IR_DESTINATION} -c ${SRC}
    )
    list(APPEND SHADER_IR_TARGETS METAL_${SHADER_INTERMED_TARGET}_IR)
    list(APPEND SHADER_IR ${SHADER_IR_DESTINATION})
endforeach ()

add_custom_target(METAL_SYMBOLS
    COMMAND xcrun -sdk macosx metal -frecord-sources=flat ${SHADER_SRC})


# Compile to metallib
add_custom_target(METAL_SHADERS
    COMMAND cp default.metallib ../../
    COMMAND cp default.metallibsym ../../)
    #COMMAND xcrun -sdk macosx metallib -o ${CMAKE_CURRENT_BINARY_DIR}/../default.metallib ${SHADER_IR})

add_dependencies(METAL_SHADERS ${SHADER_IR_TARGETS} METAL_SYMBOLS)