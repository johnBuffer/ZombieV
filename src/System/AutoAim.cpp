#include "System/AutoAim.hpp"
#include <cmath>

AutoAim::AutoAim() :
    m_target(ENTITY_NULL),
    m_dotDist(0.0f),
    m_speed(0.0f)
{

}

AutoAim::AutoAim(float speed) :
    m_speed(speed)
{

}

void AutoAim::setAimingEntity(EntityID entity)
{
    m_aimingEntity = entity;
}

void AutoAim::setTarget(EntityID entity)
{
    m_target = entity;
}


void AutoAim::update(GameWorld& world, float dt)
{
    if (m_target)
    {
        WorldEntity* aimingEntity = world.getEntityByID(m_aimingEntity);
        WorldEntity* target       = world.getEntityByID(m_target);

        float angle = aimingEntity->getAngle();
        Vec2 vTarget(target->getCoord(), aimingEntity->getCoord());
        Vec2 direction(cos(angle), sin(angle));
        Vec2 directionNormal(-direction.y, direction.x);

        float invDist = 1.0f/vTarget.getNorm();
        float vx = vTarget.x*invDist;
        float vy = vTarget.y*invDist;

        float dot2 = vx*directionNormal.x + vy*directionNormal.y;

        m_dotDist = std::abs(dot2);
        float coeff = m_speed * m_dotDist;

        m_deltaAngle = dot2<0?-coeff:coeff;
    }
    else
        m_deltaAngle = 0.0f;
}

void AutoAim::setSpeed(float speed)
{
    m_speed = speed;
}

float AutoAim::getDelta() const
{
    return m_deltaAngle;
}

float AutoAim::getDotDist() const
{
    return m_dotDist;
}

