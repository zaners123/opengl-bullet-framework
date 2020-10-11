# My OpenGL/Bullet code

Some really good (fine, they're just okay) OpenGL/Bullet code examples.

I'd recommend first taking a look at the ```/nonEuclidean/main.cpp``` file, since that's the most recent of these ones. It relies mostly on ```/libs/node/```, which is what I would consider the most useful part of this whole code pile.

Has stuff like:
 - Instancing
 - non-euclidean portals
 - Grass fields
 - Spheres
 - Texturing

## Setting up / Running it
Run these commands to get this project working:

### Install CMake and required libraries:
If you are on any Debian-based distro, this is how (Ubuntu 20 recommended)
```
sudo apt install cmake libgl1-mesa-dev libdevil-dev libglm-dev libglfw3-dev libglew-dev libespeak-dev libassimp-dev libglfw3 libglew2.1 libespeak1 libassimp5 libglu1-mesa-dev freeglut3-dev mesa-common-dev libbullet-dev
```
If you are not on a Debian-based distro your package manager will likely have equivalents to most/all of these. If you're not using Linux at all, maybe consider switching, or learn how to install these.

### Compiling
```
cmake --build cmake-build-debug --target all -- -j 4
```
To change the project you wanna use/compile, uncomment its ```add_executable``` line in the CMakeLists.txt file.
### Running
```
cmake-build-debug/opengl
```
A fullscreen 3D environment should show up.

Thank you!
