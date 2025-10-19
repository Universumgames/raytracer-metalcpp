if(USE_SHADER_METAL)
    add_subdirectory(metal)
    add_dependencies(${PROJECT_NAME} METAL_SHADERS)
endif ()

if(USE_SHADER_CUDA)
    add_subdirectory(cuda)
endif ()