#include "level1.h"

#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>

Level1::Level1(QObject *parent)
    : QObject{parent}
{
    // create body definition
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(0, 0);
    bodyDef.angle = 0;

    // add the body to the world
    b2Body* body = world.CreateBody(&bodyDef);

    // add a box fixture to the body
    b2PolygonShape box;
    box.SetAsBox(1, 2);
    body->CreateFixture(&box, 0);
}
