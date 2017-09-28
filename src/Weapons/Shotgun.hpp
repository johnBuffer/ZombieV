#ifndef SHOTGUN_HPP_INCLUDED
#define SHOTGUN_HPP_INCLUDED

#include "WeaponStatics.hpp"

class Shotgun : public WeaponStatics<Shotgun>
{
public:
    Shotgun();

    bool fire(GameWorld* world, WorldEntity* entity);
    void reload();
    void update();

    static void init();
};

#endif // SHOTGUN_HPP_INCLUDED
