#ifndef WORLDENTITY_HPP_INCLUDED
#define WORLDENTITY_HPP_INCLUDED

#include "UnitedEngine/U_2DBody.h"
#include <cstdlib>

class GameWorld;

/// Represents every physical object in the GameWorld
class WorldEntity
{
public:
    WorldEntity();

    WorldEntity(double x, double y, double angle);
    U_2DBody&  getBody();

    const U_2DCoord& getCoord() const;
    const size_t&    getID()    const;
    bool             isEnemy()  const;
    double           getAngle() const;
    bool             isAlive()  const {return __life>0;}

    void setEnemy();
    void addLife(double life) {__life += life;}
    void setLife(double life) {__life =  life;}
    void resetTime() {__time = rand()%1000;}

    virtual void update(GameWorld& world)=0;
    virtual void render()=0;

protected:
    /// Engine data
    size_t    __id;

    /// Physical data
    U_2DBody  __body;
    double    __angle;
    double    __time;
    double    __life;

    bool      __isEnnemy;

private:
    static size_t __entityCount;

};

#endif // WORLDENTITY_HPP_INCLUDED
