#ifndef AUTOAIM_HPP_INCLUDED
#define AUTOAIM_HPP_INCLUDED

#include "System/WorldEntity.hpp"
#include "System/GameWorld.hpp"

class AutoAim
{
public:
    AutoAim();
    AutoAim(float speed);

    void update(GameWorld& world, float dt);
    void setAimingEntity(EntityID entity);
    void setTarget(EntityID entity);
    void setSpeed(float speed);

    float getDelta() const;
    float getDotDist() const;

private:
    EntityID m_aimingEntity;
    EntityID m_target;

    float m_deltaAngle;
    float m_dotDist;
    float m_speed;
};

#endif // AUTOAIM_HPP_INCLUDED
