#ifndef WORLDENTITY_HPP_INCLUDED
#define WORLDENTITY_HPP_INCLUDED

#include "UnitedEngine/U_2DBody.h"
#include "EntityTypes.hpp"
#include <cstdlib>

#define SCALE 1.0f

class GameWorld;

/// Represents every object in the GameWorld
class WorldEntity
{
public:
    WorldEntity();
    WorldEntity(float x, float y, float angle);
    virtual ~WorldEntity()=0;

    bool          needsPhysics() const;
    float         getAngle()     const;
    U_2DBody&     getBody();
    const Vec2&   getCoord()     const;
    EntityTypes   getType()      const;
    virtual bool  isDone()       const=0;
    const size_t& getID()        const;

    void setEnemy();
    void resetTime();
    void addLife(float life);
    void setLife(float life);
    void setNext(WorldEntity* entity);
    void setPrev(WorldEntity* entity);

    WorldEntity* getNext() {return _next;}
    WorldEntity* getPrev() {return _prev;}

    virtual void initPhysics(GameWorld* world)=0;
    virtual void hit(WorldEntity* entity, GameWorld* gameWorld) {};
    virtual void update(GameWorld& world)=0;
    virtual void render()=0;

protected:
    /// Engine data
    size_t       _id;
    WorldEntity* _next;
    WorldEntity* _prev;

    /// Physical data
    U_2DBody _body;
    float    _angle;
    float    _time;
    float    _life;
    bool     _needsPhysics;

    EntityTypes _type;

private:
    static size_t _entityCount;

};

#endif // WORLDENTITY_HPP_INCLUDED
