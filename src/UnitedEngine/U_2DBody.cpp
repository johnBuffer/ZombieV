#include "UnitedEngine/U_2DBody.h"
#include "System/WorldEntity.hpp"
#include <cmath>
#include <iostream>

U_2DBody::U_2DBody()
{
    init(Vec2(), 0, false);
}

U_2DBody::U_2DBody(Vec2 position, float mass, bool included)
{
    init(position, mass, included);
    m_radius = 0.0;
}

void U_2DBody::init(Vec2 position, float mass, bool included)
{
    m_position = position;
    m_lastPosition = position;
    m_mass = mass;
    m_acceleration = Vec2();
    m_static = false;
    m_included = false;
    _pressure = Vec2();
    m_gridChange = true;
}

void U_2DBody::move2D(const Vec2& vec)
{
    if (!m_static)
        m_position.move2D(vec);
}

void U_2DBody::moveLastPos2D(const Vec2& vec)
{
    m_lastPosition.move2D(vec);
}

void U_2DBody::accelerate2D(const Vec2& vec)
{
    m_acceleration.move2D(vec);
}

void U_2DBody::accelerate2D(float ax, float ay)
{
    m_acceleration.x += ax;
    m_acceleration.y += ay;
}

void U_2DBody::updatePosition(float timeStep)
{
    if (!m_static)
    {
        Vec2 newPosition;
        newPosition.x = m_position.x+(m_position.x-m_lastPosition.x)+timeStep*m_acceleration.x;
        newPosition.y = m_position.y+(m_position.y-m_lastPosition.y)+timeStep*m_acceleration.y;

        m_lastPosition = m_position;
        m_position = newPosition;
    }
    else
    {
        m_position = m_lastPosition;
    }

    m_acceleration = {};
    //m_pressure = 0;
}

Vec2 U_2DBody::getVelocity() const
{
    return Vec2(m_position.x - m_lastPosition.x, m_position.y - m_lastPosition.y);
}

void U_2DBody::setEntity(WorldEntity* entity)
{
    _entity = entity;
}

WorldEntity* U_2DBody::getEntity()
{
    return _entity;
}

void U_2DBody::setPosition(const Vec2& pos)
{
    m_position = pos;
}

void U_2DBody::setX(float x)
{
    m_position.x = x;
}

void U_2DBody::setY(float y)
{
    m_position.y = y;
}

void U_2DBody::stop()
{
    m_lastPosition = m_position;
}

void U_2DBody::setStatic(bool isStatic)
{
    m_static = isStatic;
}

void U_2DBody::setPressure(Vec2 pressure)
{
    _pressure = pressure;
}

void U_2DBody::setMass(float mass)
{
    m_mass = mass;
}

void U_2DBody::setRadius(float radius)
{
    m_radius = radius;
}

void U_2DBody::addPressure(Vec2 pressure)
{
    _pressure.x += pressure.x;
    _pressure.y += pressure.y;
}

const Vec2& U_2DBody::getPosition() const
{
    return m_position;
}

bool U_2DBody::isStatic() const
{
    return m_static;
}

bool U_2DBody::isIncluded() const
{
    return m_included;
}

float U_2DBody::getMass() const
{
    return m_mass;
}

float U_2DBody::getRadius() const
{
    return m_radius;
}

float U_2DBody::getAngle(U_2DBody* body)
{
    Vec2 v = m_position-body->getPosition();

    float dist = v.getNorm();
    float angle = acos(v.x/dist);

    return v.y>0?angle:-angle;
}

void U_2DBody::move2D(float vx, float vy)
{
    m_position.x += vx;
    m_position.y += vy;
}

void U_2DBody::setPosition(float x, float y)
{
    m_position.x = x;
    m_position.y = y;
}


