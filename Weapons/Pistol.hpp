#ifndef PISTOL_HPP_INCLUDED
#define PISTOL_HPP_INCLUDED

#include "WeaponStatics.hpp"

class Pistol : public WeaponStatics<Pistol>
{
public:
    Pistol();

    bool fire(GameWorld* world, WorldEntity* entity);
    void reload();
    void update();

    static void init();
};

#endif // PISTOL_HPP_INCLUDED
