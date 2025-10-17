if(APPLE)
    set(USE_METAL_EXTENDED OFF)
    add_subdirectory(metal-cmake)
endif ()

# TODO if windows / nvida for cuda