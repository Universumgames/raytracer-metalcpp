if(USE_SHADER_METAL)
    set(USE_METAL_EXTENDED OFF)
    add_subdirectory(metal-cmake)
    add_dependencies(${PROJECT_NAME} METAL_CPP)
    target_link_libraries(${PROJECT_NAME} PUBLIC METAL_CPP)
endif ()

if(USE_SHADER_CUDA)
    set_target_properties(${PROJECT_NAME} PROPERTIES
            CUDA_SEPARABLE_COMPILATION ON)
    target_link_libraries(${PROJECT_NAME} PUBLIC
            CUDA::nvrtc
            CUDA::cuda_driver
    )

    target_compile_options(${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:--extended-lambda>)

    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17 cuda_std_17)
endif ()