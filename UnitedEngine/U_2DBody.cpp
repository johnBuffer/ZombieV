#include "U_2DBody.h"
#include "../WorldEntity.hpp"
#include <iostream>

U_2DBody::U_2DBody()
{
    init(U_2DCoord(), 0, false);
}

U_2DBody::U_2DBody(U_2DCoord position, double mass, bool included)
{
    init(position, mass, included);
}

void U_2DBody::init(U_2DCoord position, double mass, bool included)
{
    m_position = position;
    m_lastPosition = position;
    m_mass = mass;
    m_acceleration = U_2DCoord();
    m_static = false;
    m_included = false;
    __pressure = U_2DCoord();
    m_gridChange = true;
}

void U_2DBody::move2D(U_2DCoord vec)
{
    if (!m_static)
        m_position.move2D(vec);
}

void U_2DBody::moveLastPos2D(U_2DCoord vec)
{
    m_lastPosition.move2D(vec);
}

void U_2DBody::accelerate2D(U_2DCoord vec)
{
    m_acceleration.move2D(vec);
}

void U_2DBody::updatePosition(double timeStep)
{
    U_2DCoord newPosition;
    newPosition.x = m_position.x+(m_position.x-m_lastPosition.x)+timeStep*m_acceleration.x;
    newPosition.y = m_position.y+(m_position.y-m_lastPosition.y)+timeStep*m_acceleration.y;

    if (!m_static)
    {
        m_lastPosition = m_position;
        m_position = newPosition;
    }

    m_acceleration = {};
    //m_pressure = 0;
}

U_2DCoord U_2DBody::getVelocity() const
{
    return U_2DCoord(m_position.x - m_lastPosition.x, m_position.y - m_lastPosition.y);
}

void U_2DBody::setEntity(WorldEntity* entity)
{
    __entity = entity;
}


WorldEntity* U_2DBody::getEntity()
{
    return __entity;
}

void U_2DBody::setPosition(U_2DCoord pos)
{
    m_position = pos;
}

void U_2DBody::setX(double x)
{
    m_position.x = x;
}

void U_2DBody::setY(double y)
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

void U_2DBody::setPressure(U_2DCoord pressure)
{
    __pressure = pressure;
}

void U_2DBody::addPressure(U_2DCoord pressure)
{
    __pressure.x += pressure.x;
    __pressure.y += pressure.y;
}

const U_2DCoord& U_2DBody::getPosition() const
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

double U_2DBody::getMass() const
{
    return m_mass;
}

U_2DCoord U_2DBody::getPressure()
{
    return __pressure;
}


