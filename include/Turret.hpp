#ifndef TURRET_HPP_INCLUDED
#define TURRET_HPP_INCLUDED

#include "System/StandardEntity.hpp"
#include "LightEngine/LightEngine.hpp"
#include "System/Cooldown.hpp"
#include "System/AutoAim.hpp"

class Turret : public StandardEntity<Turret>
{
public:
    Turret();
    Turret(float x, float y);

    void initPhysics(GameWorld* world);
    void update(GameWorld& world);
    void render();

    void setTarget(EntityID entity);

    bool isDone() const {return false;}

    static void init();

    enum TurretState
    {
        IDLE,
        AIMING,
        SHOOTING
    };

private:
    Cooldown m_fireCooldown;
    float    m_accuracy;

    Light* light;

    TurretState m_currentState;
    EntityID    m_target;
    AutoAim     m_autoAim;

    EntityID getTarget(GameWorld* world) const;
    void fire(GameWorld* world);

    BodyID m_barrelBody;

    static size_t s_textureID;
    static size_t s_shootSoundID;
};

#endif // TURRET_HPP_INCLUDED
