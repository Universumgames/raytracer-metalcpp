# TODO setup steps for metal

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../shader/metal ${CMAKE_CURRENT_BINARY_DIR}/shader/metal)

add_custom_target(SHADER_LIB METAL_SHADERS)