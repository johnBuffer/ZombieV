#include "WorldEntity.hpp"

#include <iostream>

size_t WorldEntity::_entityCount = 0;

WorldEntity::WorldEntity():
    _id(WorldEntity::_entityCount++),
    _body(Vec2(0.0, 0.0), 1.0, false),
    _angle(0.0)
{
    _body.setEntity(this);
    _needsPhysics = true;
}

WorldEntity::WorldEntity(float x, float y, float angle):
    _id(WorldEntity::_entityCount++),
    _body(Vec2(x, y), 1.0, false),
    _angle(0.0)
{
    _body.setEntity(this);
    _needsPhysics = true;
}

WorldEntity::~WorldEntity()
{

}

U_2DBody& WorldEntity::getBody()
{
    return _body;
}

const Vec2& WorldEntity::getCoord() const
{
    return _body.getPosition();
}

const size_t& WorldEntity::getID() const
{
    return _id;
}

float WorldEntity::getAngle() const
{
    return _angle;
}

EntityTypes WorldEntity::getType() const
{
    return _type;
}

bool WorldEntity::needsPhysics() const
{
    return _needsPhysics;
}

void WorldEntity::addLife(float life)
{
    _life += life;
}

void WorldEntity::setLife(float life)
{
    _life = life;
}

void WorldEntity::setNext(WorldEntity* entity)
{
    _next = entity;
}

void WorldEntity::setPrev(WorldEntity* entity)
{
    _prev = entity;
}

void WorldEntity::resetTime()
{
    _time = 0;
}
