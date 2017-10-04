#include "System/WorldEntity.hpp"

#include <iostream>

size_t WorldEntity::_entityCount = 0;

WorldEntity::WorldEntity():
    _id(WorldEntity::_entityCount++),
    _body(Vec2(0.0, 0.0), 1.0, false),
    _angle(0.0)
{
    _body.setEntity(this);
    _isDying = false;
}

WorldEntity::WorldEntity(float x, float y, float angle):
    _id(WorldEntity::_entityCount++),
    _body(Vec2(x, y), 1.0, false),
    _angle(angle)
{
    _body.setEntity(this);
    _isDying = false;
}

WorldEntity::WorldEntity(const WorldEntity& entity) :
    _body(Vec2(0.0f, 0.0f), 1.0f)
{
    _id = WorldEntity::_entityCount++;
    _body.setPosition(entity.getCoord());
    _body.stop();
    _body.setEntity(this);

    _angle   = entity._angle;
    _time    = entity._time;
    _life    = entity._life;
    _isDying = entity._isDying;
    _type    = entity._type;
}

WorldEntity::~WorldEntity()
{

}

void WorldEntity::kill()
{
    // deleter
}

void WorldEntity::setDying()
{
    _isDying = true;
}

bool WorldEntity::isDying() const
{
    return _isDying;
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

void WorldEntity::addLife(float life)
{
    _life += life;
}

void WorldEntity::setLife(float life)
{
    _life = life;
}

void WorldEntity::resetTime()
{
    _time = 0;
}
