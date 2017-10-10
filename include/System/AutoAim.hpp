#ifndef AUTOAIM_HPP_INCLUDED
#define AUTOAIM_HPP_INCLUDED

#include "System/WorldEntity.hpp"

class AutoAim
{
public:
    AutoAim();
    AutoAim(WorldEntity* entity, float speed);

    void update(float dt);
    void setTarget(WorldEntity* entity);
    void setSpeed(float speed);

    float getDelta() const;
    float getDotDist() const;

private:
    WorldEntity* m_aimingEntity;
    WorldEntity* m_target;

    float m_deltaAngle;
    float m_dotDist;
    float m_speed;
};

#endif // AUTOAIM_HPP_INCLUDED
