#include "level.h"

Level::Level()
    : world(GRAVITY)
{ }

b2World* Level::getWorld() {
    return &world;
}

b2Body* Level::getBodies() {
    return world.GetBodyList();
}

int Level::getBodyCount() {
    return world.GetBodyCount();
}
