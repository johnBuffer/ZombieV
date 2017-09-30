#ifndef TURRET_HPP_INCLUDED
#define TURRET_HPP_INCLUDED

#include "WorldEntity.hpp"
#include "DrawableEntity.hpp"
#include "LightEngine/LightEngine.hpp"

class Turret : public WorldEntity, public DrawableEntity<Turret>
{
public:
    Turret();
    Turret(float x, float y);

    void initPhysics(GameWorld* world);
    void update(GameWorld& world);
    void render();

    void setTarget(WorldEntity* entity) {m_target = entity;}

    bool isDone() const {return false;}

    static void init();

    enum TurretState
    {
        IDLE,
        AIMING,
        SHOOTING
    };

private:
    float m_cooldown, m_currentCooldown;
    float m_accuracy;

    Light* light;

    TurretState m_currentState;
    WorldEntity* m_target;

    WorldEntity* getTarget(GameWorld* world) const;
    void fire(GameWorld* world);

    U_2DBody* m_body;

    static size_t s_textureID;
    static size_t s_shootSoundID;
};

#endif // TURRET_HPP_INCLUDED
