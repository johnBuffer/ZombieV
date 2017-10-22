#ifndef WALL_HPP_INCLUDED
#define WALL_HPP_INCLUDED

#include "System/WorldEntity.hpp"

class Wall : public WorldEntity
{
public:
    Wall();
    Wall(Vec2 pos, double angle);

    void initPhysics(GameWorld* world);
    void update(GameWorld& world);
    void render();

    bool isDone() const {return false;}
    static void init();

private:

};

#endif // WALL_HPP_INCLUDED
