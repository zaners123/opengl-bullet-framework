# Report - How I Considered Different Components of BulletPhysics

## The Node class

The hardest part of incorporating bullet physics was "bridging the gap" of OpenGL's buffers and BulletPhysics's btRigidBody objects.

This was done by creating a Node object which is
both an OpenGL object and a btRigidBody.

Many common operations, such as "move","scale", and "rotate"
affect both the btRigidBody and the OpenGL visuals, this way the physics line-up with what you see.

### Benefits of Node hierarchy

This node hierarchy has many benefits, such as:
- It's easy to update any attribute of an object, such as
    - Texture with node->setTexture("../wood.jpeg")
    - Size, such as node->scale(10);
    - Mass, such as node->setMass(5) or node->setFixed()
    - Shape, such as .setShape(NodeBuilder::sphere)
- It's easy to add new shapes, such as:
    - Sphere.h being only 25 lines of code not counting comments
    - Circle.h being only 17 lines of code
- Complex code, such as creating, allocating, and filling an OpenGL buffer can be abstracted 
- Making something complex like a SkyBox can be done with only 6 lines of code
```C++
auto* skybox = NodeBuilder::start()//uses a builder for creation
    .setShape(NodeBuilder::sphere)
    ->setTexture("../sky.jpeg")
    ->build();
skybox->scale(3000);//large enough to hold the entire world
rootNode->push(skybox,false);//The "false" means it won't have any physics 
```

This abstraction allows for convenient horizontal and vertical code expansion

### Masses of individual objects

Due to this node hierarchy, extending from it is simple. The node hierarchy has a setMass function, which when called, replaces a shape's mass and inertia.

Currently, all fruit is given the same mass, but it could be set simply by calling something like setMass(100)

## Setting up the BulletPhysics World

The BulletPhysics world is made by a default physics environment with -9.8m/s gravity. Around 60 times per second, this physics world discretely updates (objects fall, bounce, roll, etc), then all of the Nodes' positions (location and orientation) are updated reflecting the new locations.

## Creating the bowl

Since BulletPhysics seems to have no way of representing movable concave shapes, the bowl was made by a combination of multiple convex shapes, which could be combined to create one concave shape.