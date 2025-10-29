
file(GLOB SHADER_SRC *.cu)

add_custom_target(
        COPIED_CUDA
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SHADER_SRC} ${CMAKE_CURRENT_BINARY_DIR}
)