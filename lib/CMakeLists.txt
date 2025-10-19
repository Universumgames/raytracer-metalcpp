if(USE_SHADER_METAL)
    set(USE_METAL_EXTENDED OFF)
    add_subdirectory(metal-cmake)
    add_dependencies(${PROJECT_NAME} METAL_CPP)
    target_link_libraries(${PROJECT_NAME} PUBLIC METAL_CPP)
endif ()

# TODO if windows / nvida for cuda