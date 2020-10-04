# My OpenGL/Bullet code

Some really good (fine, they're just okay) OpenGL/Bullet code examples.

Has stuff like:
 - Instancing
 - non-euclidean portals
 - Grass fields
 - Spheres
 - Texturing

## Setting up / Running it
Run these commands to get this project working:

Install required libraries and CMake:
```sudo apt install cmake libgl1-mesa-dev libdevil-dev libglm-dev libglfw3-dev libglew-dev libespeak-dev libassimp-dev libglfw3 libglew2.1 libespeak1 libassimp5 libglu1-mesa-dev freeglut3-dev mesa-common-dev libbullet-dev```

Compile with something like:
```cmake --build cmake-build-debug --target all -- -j 4```

Run with something like:
```cmake-build-debug/opengl```

A fullscreen 3D environment should show up.

To change the project you wanna use/compile, uncomment its ```add_executable``` line in the CMakeLists.txt file.

Thank you!
