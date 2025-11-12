# Multi Shader Language Raytracer

## Summary
Our goal was to create a relatively simple raytracer in C++.
Out of curiosity we wanted to create not only one simple Raytracer but extend this idea for this project to raytrace in
C++ using a sequential implementation as comparison to a Metal Shader Implementation.
Depending on which operating system you are on, cmake automatically fetches most dependencies and compiles with the available shader (CUDA on Windows and Metal on MacOS, since no other option is either possible or probable).
If you want to run this project without any shader and just try to CPU-raytrace you can even compile this project on linux.
The only dependencies required for this project to compile is SFML (libsfml-dev) and their dependencies. 
There is a simple make target provided to install the required dependencies with the linux apt package manager (`make install_deps_apt`).

We did not come up with all the formulas and ideas on our own and used some libraries as well as learning resources to get this project so far.
For a detailed list of all resources and libraries we used along the journey we compiled an overview in [
`Resources.md`](./Resources.md).

An important note concerning the shader programming and raytracing with shaders is the use of the compute
pipelines.
We did not use the rendering pipeline and their raytracing features, we used the normal compute pipelines and needed to
implement our own data structure like vertex buffers.

## Configuration and Project Structure

Every aspect of the raytracing application can be configured when calling the executable, a detailed list of all
available options can be found when calling the executable with the `--help` flag.
The defaults are set in the [`main.cpp`](src/main.cpp) file.
The C++ source files can be found in the [`src/`](src/) folder, the shaders in the [`shaders/`](shaders/) folder. Needed
assets like scene files and 3D models are located in the [`scene/`](scene/) directory.

## Scene Configuration

This raytracer uses scene configuration files in JSON format to define the objects, materials, lights, and camera
settings for rendering.
These configuration files allow users to customize the scene being rendered by specifying various parameters.
The format for these json files is described in detail in the [`scene/README.md`](scene/README.md) file.

## Features

This raytracer has multiple implementations for raytracing: Sequential C++ implementation, OpenMP Multithreading and a
Metal Shader implementation.
Each implementation works identically on a high level, but the underlying logic had to be adapted to the respective
platform.
The scenes that can be rendered are defined in JSON files by referencing 3D models in STL format.
Multiple bounces and multiple rays per pixel (samples) are supported to achieve good rendering effects, but each object
only supports a single color.
Upon rendering the scene the objects are loaded and prepared for raytracing. Part of this preparation is building a
Bounding Volume Hierarchy (BVH) to accelerate the ray-object intersection tests.

## Inner working

The raytracer works by simulating the path of light rays as they interact with objects in a 3D scene.
To achieve this, for each pixel on the screen multiple rays are cast into the scene with slightly different directions
to simulate anti-aliasing and soft shadows.
Each ray checks for intersections with objects in the scene using the BVH for acceleration.
When a ray intersects an object, the color of the intersection is stored and the ray is reflected or refracted based on
the `specularIntensity` of the object.
If the ray hits a light source, the color of the light is stored separately and the ray tracing for that path ends.
This process is repeated for a specified number of bounces, allowing for multiple reflections and refractions.
After all the bounces are finished for a ray, the colors from each hit are multiplied (using color values 0.0-1.0) to
determine the ray color.
Finally after all the rays are traced, the average color of all rays (the samples) per pixel is calculated and written
to the output image.
