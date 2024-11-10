
# CCaster

This project is a basic raytracer implemented in C, designed to render 3D scenes containing various geometric objects such as spheres and planes. It supports simple lighting, color, and shading techniques, resulting in a bitmap image output. It was written for the masters course "Parallel Computing".

## Features

- **Bitmap Image Creation**: Uses `bitmap.c` to create, load, and save images in the BMP format. Pixels are represented in RGB format and support antialiasing for smoother images.
- **Camera Setup**: Defines a virtual camera in `camera.c` to capture the scene, allowing configuration of position, orientation, and field of view. The camera calculates the direction for each ray based on screen coordinates.
- **Color Handling**: `color.c` defines various colors and allows color manipulation, including addition and scaling for rendering effects.
- **Lighting**: Implements basic ambient and directional lighting using `light.c`. Lights can be positioned in 3D space, affecting object colors based on distance and angle.
- **Geometric Objects**: Includes `sphere.c` and `plane.c` for handling spheres and planes, each supporting intersection calculations, color definitions, and specular highlights.
- **Raytracing Algorithm**: `raytracer.c` traces rays from the camera through each pixel, calculating intersections with scene objects. The algorithm applies lighting and shading based on the object's position, color, and light intensity, creating realistic visuals with ambient, diffuse, and specular lighting.
- **Scene Management**: `scene.c` manages objects within the scene, including adding and removing spheres, planes, and lights.

## Getting Started

### Prerequisites
- C Compiler (e.g., GCC)
- Standard C Library

### Building the Project
To compile the project, use a C compiler to compile all `.c` files. For example:
```bash
gcc -o raytracer main.c bitmap.c camera.c color.c light.c plane.c raytracer.c scene.c sphere.c
```

### Running the Program
Run the executable to render the scene and output a `rendered_scene.bmp` file.
```bash
./raytracer
```

### Output
The program renders a scene with objects, lighting, and camera settings defined in `main.c`. The output image is saved as `rendered_scene.bmp`.

## Future Improvements
- Multiple lights, shadow rendering, reflections, and potentially real-time rendering.
- Additional shapes, materials, and textures for more complex scenes.

## License
This project is licensed under the MIT License.
