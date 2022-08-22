#ifndef LEVEL_H
#define LEVEL_H

#include <Box2D/Dynamics/b2World.h>

const b2Vec2 GRAVITY(0, -9.81);

class Level
{
public:
    Level();
    b2World* getWorld();
    b2Body* getBodies();
    int getBodyCount();

protected:
    b2World world;
};

#endif // LEVEL_H
