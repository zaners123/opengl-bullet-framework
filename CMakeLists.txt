cmake_minimum_required(VERSION 3.15)
project(opengl)

set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

INCLUDE_DIRECTORIES(${BULLET_PHYSICS_SOURCE_DIR}/src)
LINK_LIBRARIES(BulletDynamics BulletCollision LinearMath)

#minecarft
#add_executable(opengl minecraft/main.cpp camera/camera.cpp
#        minecraft/WorldCube/PlaneMesh.cpp minecraft/WorldCube/Chunk.cpp minecraft/WorldCube/World.cpp
#        minecraft/WorldHex/PlaneMesh.cpp minecraft/WorldHex/Chunk.cpp minecraft/WorldHex/World.cpp
#        minecraft/libs/fastnoise/FastNoise.cpp minecraft/PhysicsCamera.cpp minecraft/PhysicsObject.cpp)

add_executable(opengl
        nonEuclidean/main.cpp
        libs/camera/camera.cpp
        libs/node/Maze.h libs/Timer.h libs/node/Plane.h libs/node/PointMaze.h libs/node/Cube.h libs/node/Node.h libs/node/Drawable.h libs/node/Collidable.h libs/node/NodeVector.h libs/node/Portal.h libs/node/SimpleNode.h libs/node/Circle.h libs/node/Cylinder.h libs/node/Orientable.h libs/node/Cube.h libs/node/TexturedNode.h libs/node/QuarterBowl.h libs/node/NodeBuilder.h libs/node/Tree.h
        libs/node/meshes/Grass.h
        )

#add_executable(opengl mazeGame/main.cpp camera/camera.cpp)
# 499 Assmt 1/3a
#add_executable(opengl colorfield/main.cpp camera/camera.cpp)
# 499 assmnt 2/3b
#Bunny/Army
#add_executable(opengl BunnyModel/main.cpp libs/objloader/objloader.cpp BunnyModel/shader.cpp camera/camera.cpp libs/objloader/objloader.cpp)
#add_executable(opengl mazeGame/main.cpp)

#sudo apt install libgl1-mesa-dev libdevil-dev libglm-dev libglfw3-dev libglew-dev libespeak-dev libassimp-dev libglfw3 libglew2.1 libespeak1 libassimp5 libglu1-mesa-dev freeglut3-dev mesa-common-dev libbullet-dev

#include_directories(/usr/include/bullet/)
include_directories(${BULLET_INCLUDE_DIR})

target_link_libraries(opengl glut GLU GL GLEW espeak assimp IL)

