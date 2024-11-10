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
        src/color.c
        include/color.h
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
