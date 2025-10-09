cmake_minimum_required(VERSION 3.29)
project(CCaster)

set(CMAKE_CXX_STANDARD 20)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config.ini
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(CCaster src/main.c
        src/bitmap.c
        include/bitmap.h
        src/camera.c
        include/camera.h
        src/configservice.c
        include/configservice.h
        src/light.c
        include/light.h
        src/plane.c
        include/plane.h
        src/ray.c
        include/ray.h
        src/raytracer.c
        include/raytracer.h
        src/scene.c
        include/scene.h
        src/sphere.c
        include/sphere.h
        src/vector3d.c
        include/vector3d.h
)

# ----- MacOS -----

include_directories(/opt/homebrew/include)
add_library( csfml-system SHARED IMPORTED )
set_target_properties( csfml-system PROPERTIES IMPORTED_LOCATION /opt/homebrew/lib/libcsfml-system.dylib )
add_library( csfml-window SHARED IMPORTED )
set_target_properties( csfml-window PROPERTIES IMPORTED_LOCATION /opt/homebrew/lib/libcsfml-window.dylib )
add_library( csfml-graphics SHARED IMPORTED )
set_target_properties( csfml-graphics PROPERTIES IMPORTED_LOCATION /opt/homebrew/lib/libcsfml-graphics.dylib )
target_link_libraries(CCaster csfml-graphics csfml-window csfml-system)
