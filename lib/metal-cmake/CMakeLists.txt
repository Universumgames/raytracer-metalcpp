# Library definition
add_library(METAL_CPP
        ${CMAKE_CURRENT_SOURCE_DIR}/definition.cpp
)

if(USE_METAL_EXTENDED)
    set(METAL_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/metal-cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/metal-cpp-extensions")
    set(METAL_LINK_FRAMEWORKS "-framework Metal"
            "-framework MetalKit"
            "-framework Foundation"
            "-framework QuartzCore")
    add_compile_definitions(METAL_EXTENDED)
else ()
    set(METAL_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/metal-cpp")
    set(METAL_LINK_FRAMEWORKS "-framework Metal"
            "-framework Foundation")
endif ()

# Metal cpp headers
target_include_directories(METAL_CPP PUBLIC ${METAL_INCLUDE_DIRS})

# Metal cpp library (linker)
target_link_libraries(METAL_CPP ${METAL_LINK_FRAMEWORKS})

