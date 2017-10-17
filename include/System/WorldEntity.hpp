#ifndef WORLDENTITY_HPP_INCLUDED
#define WORLDENTITY_HPP_INCLUDED

#include "UnitedEngine/U_2DBody.h"
#include "UnitedEngine/Types.hpp"
#include "EntityTypes.hpp"
#include <cstdlib>

#define SCALE 1.0f

typedef WorldEntity* WorldEntityPtr;

class GameWorld;

/// Represents every object in the GameWorld
class WorldEntity
{
public:
    WorldEntity();
    WorldEntity(float x, float y, float angle);
    WorldEntity(const WorldEntity& entity);

    virtual ~WorldEntity();

    bool          isDying()      const;
    float         getAngle()     const;
    U_2DBody&     getBody();
    const Vec2&   getCoord()     const;
    const Vec2&   getBodyCoord() const;
    EntityTypes   getType()      const;
    virtual bool  isDone()       const=0;
    const size_t& getID()        const;

    void move(float vx, float vy);
    void setDying();
    void setEnemy();
    void resetTime();
    void addLife(float life);
    void setLife(float life);

    virtual void kill();
    virtual void initPhysics(GameWorld* world)=0;
    virtual void hit(WorldEntity* entity, GameWorld* gameWorld) {};
    virtual void update(GameWorld& world)=0;
    virtual void render()=0;

protected:
    /// Engine data
    size_t _id;

    /// Physical data
    Vec2   m_coord;
    size_t m_bodyID;
    float  _angle;
    float  _time;
    float  _life;
    bool   _isDying;

    EntityTypes _type;

    const U_2DBody* m_thisBody() const;
    U_2DBody* m_thisBody();
    U_2DBody* m_initBody(GameWorld* world);

private:
    static size_t _entityCount;

};

#endif // WORLDENTITY_HPP_INCLUDED
