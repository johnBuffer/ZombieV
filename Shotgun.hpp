#ifndef SHOTGUN_HPP_INCLUDED
#define SHOTGUN_HPP_INCLUDED

#include "WeaponStatics.hpp"

class Shotgun : public WeaponStatics<Shotgun>
{
public:
    Shotgun();

    std::list<Bullet> fire();
    void       reload();
    void       update();

    static void subLoadTextures();
};

#endif // SHOTGUN_HPP_INCLUDED
