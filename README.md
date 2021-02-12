# TODO
Add leaves (red,yellow,maple leaves, green leaves)
Fix grass (make thinner / wavier)
Change grass texture with dirt / green texture

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
If you are on any Debian-based distro (Works on Ubuntu 16, Ubuntu 20, Mint 21, and likely on Debian), run this to install all dependencies:
```
sudo apt install git g++ cmake libgl1-mesa-dev libdevil-dev libglm-dev libglfw3-dev libglew-dev libespeak-dev libassimp-dev libglfw3 libglew2.1 libespeak1 libassimp5 libglu1-mesa-dev freeglut3-dev mesa-common-dev libbullet-dev
```
If you are not on a Debian-based distro your package manager will likely have equivalents to most/all of these.

If you're using Windows, you'll likely be able to install the equivalents of these inside of Visual Studio

### Compiling
Feel free to use an IDE to compile/run this, such as CLion by JetBrains.

Set the working directory to the base directory of this project 

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
