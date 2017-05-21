#include "WorldEntity.hpp"

#include <iostream>

size_t WorldEntity::__entityCount = 0;

WorldEntity::WorldEntity():
    __id(WorldEntity::__entityCount++),
    __body(U_2DCoord(0.0, 0.0), 1.0, false),
    __angle(0.0)
{
}

WorldEntity::WorldEntity(double x, double y, double angle):
    __id(WorldEntity::__entityCount++),
    __body(U_2DCoord(x, y), 1.0, false),
    __angle(0.0)
{
}

U_2DBody& WorldEntity::getBody()
{
    return __body;
}

const U_2DCoord& WorldEntity::getCoord() const
{
    return __body.getPosition();
}

const size_t& WorldEntity::getID() const
{
    return __id;
}

bool WorldEntity::isEnemy() const
{
    return __isEnnemy;
}

double WorldEntity::getAngle() const
{
    return __angle;
}

void WorldEntity::setEnemy()
{
    __isEnnemy = true;
}
