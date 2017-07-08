#ifndef WORLDENTITY_HPP_INCLUDED
#define WORLDENTITY_HPP_INCLUDED

#include "UnitedEngine/U_2DBody.h"
#include "EntityTypes.hpp"
#include <cstdlib>

class GameWorld;

/// Represents every physical object in the GameWorld
class WorldEntity
{
public:
    WorldEntity();
    WorldEntity(float x, float y, float angle);

    bool          needsPhysics() const {return _needsPhysics;}
    U_2DBody&     getBody();
    const Vec2&   getCoord() const;
    const size_t& getID()    const;
    float         getAngle() const;
    virtual bool  isDone()   const=0;
    EntityTypes   getType()  const;

    void setEnemy();
    void addLife(float life) {_life += life;}
    void setLife(float life) {_life =  life;}
    void resetTime() {_time = rand()%1000;}

    virtual void initPhysics(GameWorld* world)=0;

    virtual void hit(WorldEntity* entity, GameWorld* gameWorld) {};
    virtual void update(GameWorld& world)=0;
    virtual void render()=0;

protected:
    /// Engine data
    size_t    _id;

    /// Physical data
    U_2DBody  _body;
    float    _angle;
    float    _time;
    float    _life;
    bool      _needsPhysics;

    EntityTypes _type;

private:
    static size_t _entityCount;

};

#endif // WORLDENTITY_HPP_INCLUDED
