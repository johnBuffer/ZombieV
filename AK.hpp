#ifndef AK_HPP_INCLUDED
#define AK_HPP_INCLUDED

#include "WeaponStatics.hpp"

class AK : public WeaponStatics<AK>
{
public:
    AK();

    std::list<Bullet> fire();
    void       reload();
    void       update();

    static void subLoadTextures();
};

#endif // AK_HPP_INCLUDED
