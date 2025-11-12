# Scene JSON Format

This Readme describes the JSON format used for configuring scenes in the raytracer. The scene configuration includes
camera settings, objects, spheres, and lights.

- `camera`: Defines the camera's field of view
    - `fov`: The field of view in degrees
- `objects`: An array of objects in the scene
    - `fileName`: The path to the 3D model file (STL format)
    - `position`: The position of the object in 3D space as an array [x, y, z]
    - `rotation`: The rotation of the object in degrees as an array [x, y, z]
    - `color`: The color of the object in RGB format (0-255) with properties:
        - `r`: Red component
        - `g`: Green component
        - `b`: Blue component
        - `a`: Alpha (opacity) component
    - `specularIntensity`: The intensity of specular highlights (0.0 to 1.0)
    - `scale`: The scale factor for the object as an array [x, y, z]
- `spheres`: An array of spheres in the scene
    - `position`: The position of the sphere in 3D space as an array [x, y, z]
    - `radius`: The radius of the sphere
    - `color`: The color of the sphere in RGB format (0-255) with properties:
        - `r`: Red component
        - `g`: Green component
        - `b`: Blue component
        - `a`: Alpha (opacity) component
    - `specularIntensity`: The intensity of specular highlights (0.0 to 1.0)
- `lights`: An array of light sources in the scene
    - `position`: The position of the light in 3D space as an array [x, y, z]
    - `radius`: The radius of the light source
    - `emittingColor`: The color of the light in RGB format (0-255) with properties:
        - `r`: Red component
        - `g`: Green component
        - `b`: Blue component
        - `a`: Alpha (opacity) component