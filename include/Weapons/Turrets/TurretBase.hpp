#ifndef TurretBaseBASE_HPP_INCLUDED
#define TurretBaseBASE_HPP_INCLUDED

#include "WorldEntity.hpp"
#include "DrawableEntity.hpp"
#include "LightEngine/LightEngine.hpp"

class TurretBase : public WorldEntity, public DrawableEntity<TurretBase>
{
public:
    TurretBase();
    TurretBase(float x, float y);

    void initPhysics(GameWorld* world);
    void update(GameWorld& world);
    void render();

    void setTarget(WorldEntity* entity) {m_target = entity;}

    bool isDone() const {return false;}

    static void init();

    enum TurretBaseState
    {
        IDLE,
        AIMING,
        SHOOTING
    };

protected:
    float m_accuracy;
    float m_cooldown;
    float m_currentCooldown;

    Light* light;

    TurretState m_currentState;
    WorldEntity* m_target;

    virtual void fire(GameWorld* world);

    U_2DBody* m_body;

    static size_t s_textureID;
    static size_t s_shootSoundID;

    static WorldEntity* getTarget(GameWorld* world) const;
};

#endif // TurretBaseBASE_HPP_INCLUDED
