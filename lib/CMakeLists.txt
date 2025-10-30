if(USE_SHADER_METAL)
    set(USE_METAL_EXTENDED OFF)
    add_subdirectory(metal-cmake)
    add_dependencies(${PROJECT_NAME} METAL_CPP)
    target_link_libraries(${PROJECT_NAME} PUBLIC METAL_CPP)
endif ()

if(USE_SHADER_CUDA)
    find_package(CUDAToolkit REQUIRED)

    set(CMAKE_CUDA_STANDARD 20)
    set(CMAKE_CUDA_STANDARD_REQUIRED ON)

    set(CMAKE_CUDA_ARCHITECTURES 75 80 86 87 89 90 100 110 120)

    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -Wno-deprecated-gpu-targets")
    if(ENABLE_CUDA_DEBUG)
        set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -G")        # enable cuda-gdb (may significantly affect performance on some targets)
    else()
        set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -lineinfo") # add line information to all builds for debug tools (exclusive to -G option)
    endif()

    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} --extended-lambda")

    set_target_properties(${PROJECT_NAME} PROPERTIES
            CUDA_SEPARABLE_COMPILATION ON)

    target_compile_options(${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:--extended-lambda>)

    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17 cuda_std_17)

    target_include_directories(${PROJECT_NAME} PUBLIC cuda-common)

    target_link_libraries(${PROJECT_NAME} PUBLIC
            CUDA::nvrtc
            CUDA::cuda_driver
    )
endif ()