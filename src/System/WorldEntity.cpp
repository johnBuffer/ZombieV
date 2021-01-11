#include "System/WorldEntity.hpp"
#include "System/GameWorld.hpp"
#include <iostream>

uint32_t WorldEntity::_entityCount = 0;
uint32_t WorldEntity::_entityTypeCount = 0;

WorldEntity::WorldEntity():
    _id(WorldEntity::_entityCount++),
    //_body(Vec2(0.0, 0.0), 1.0, false),
    m_coord(0, 0),
    _angle(0.0)
{
    //_body.setEntity(this);
    _isDying = false;
}

WorldEntity::WorldEntity(float x, float y, float angle):
    _id(WorldEntity::_entityCount++),
    //_body(Vec2(x, y), 1.0, false),
    m_coord(x, y),
    _angle(angle)
{
    //_body.setEntity(this);
    _isDying = false;
}

WorldEntity::WorldEntity(const WorldEntity& entity)// :
    //_body(Vec2(0.0f, 0.0f), 1.0f)
{
    _id = WorldEntity::_entityCount++;
    //_body.setPosition(entity.getCoord());
    //_body.stop();
    //_body.setEntity(this);

    _angle   = entity._angle;
    _time    = entity._time;
    _life    = entity._life;
    _isDying = entity._isDying;
    _type    = entity._type;
}

WorldEntity::~WorldEntity()
{

}

void WorldEntity::move(float vx, float vy)
{
    U_2DBody* b = m_thisBody();
    b->stop();
    b->accelerate2D(vx, vy);
}

U_2DBody* WorldEntity::m_initBody(GameWorld* world)
{
    m_bodyID = static_cast<uint32_t>(world->addBody());
    U_2DBody* body = m_thisBody();

    body->setEntity(this);
    body->setPosition(m_coord);
    body->stop();

    return body;
}

void WorldEntity::setID(EntityID id)
{
    m_globalID = id;
}

const U_2DBody* WorldEntity::m_thisBody() const
{
    return U_2DCollisionManager::getBodyByID(m_bodyID);
}

U_2DBody* WorldEntity::m_thisBody()
{
    return U_2DCollisionManager::getBodyByID(m_bodyID);
}

const Vec2& WorldEntity::getBodyCoord() const
{
    return m_thisBody()->getPosition();
}

void WorldEntity::kill(GameWorld* world)
{
    // deleter
}

uint32_t WorldEntity::registerEntity()
{
	return _entityTypeCount++;
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
    return *m_thisBody();
}

const Vec2& WorldEntity::getCoord() const
{
    return m_coord;
}

EntityID WorldEntity::getID() const
{
    return m_globalID;
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
