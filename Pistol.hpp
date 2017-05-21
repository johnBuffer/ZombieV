#ifndef PISTOL_HPP_INCLUDED
#define PISTOL_HPP_INCLUDED

#include "WeaponStatics.hpp"

class Pistol : public WeaponStatics<Pistol>
{
public:
    Pistol();

    std::list<Bullet> fire();
    void       reload();
    void       update();

    static void subLoadTextures();
};

#endif // PISTOL_HPP_INCLUDED
