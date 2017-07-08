#ifndef AK_HPP_INCLUDED
#define AK_HPP_INCLUDED

#include "WeaponStatics.hpp"

class AK : public WeaponStatics<AK>
{
public:
    AK();

    bool fire(GameWorld* world, WorldEntity* entity);
    void reload();
    void update();

    static void init();

private:
    Vec2 _fireOut;
    Vec2 _bulletOut;
    Vec2 _shellsOut;
};

#endif // AK_HPP_INCLUDED
