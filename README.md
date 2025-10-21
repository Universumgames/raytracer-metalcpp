# Multi Shader Language Raytracer

## Intro
Our goal was to create a relatively simple raytracer in C++.
Out of curiosity we wanted to create not only one simple Raytracer but extend this idea for this project to raytrace not only in C++ but also CUDA (NVIDA) and Metal (Apple).
Depending on which operating system you are on, cmake automatically fetches most dependencies and compiles with the available shader (CUDA on Windows and Metal on MacOS, since no other option is either possible or probable).
If you want to run this project without any shader and just try to CPU-raytrace you can even compile this project on linux.
The only dependencies required for this project to compile is SFML (libsfml-dev) and their dependencies. 
There is a simple make target provided to install the required dependencies with the linux apt package manager (`make install_deps_apt`).

We did not come up with all the formulas and ideas on our own and used some libraries as well as learning resources to get this project so far.
For a detailed list of all resources and libraries we used along the journey we compiled an overview in [`Resources.md`](./Resources.md)

TODO
