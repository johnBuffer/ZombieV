#ifndef WEAPON_HPP_INCLUDED
#define WEAPON_HPP_INCLUDED

#include <string>
#include <list>
#include "Animation.hpp"
#include "Bullet.hpp"

enum WeaponType
{
    BULLET,
    ROCKET
};

class Weapon
{
public:
    Weapon();

    double getRecoil()   const {return __recoil;}
    double getFireDist() const {return __fireDist;}
    Animation& getShootAnimation() {return __shootAnimation;}
    Animation& getMoveAnimation()  {return __moveAnimation;}
    Animation& getIdleAnimation()  {return __idleAnimation;}

    virtual std::list<Bullet> fire()=0;
    virtual void reload()=0;
    virtual void update()=0;

    void releaseTrigger() {__releasedTrigger=true;}

protected:
    double __damage;
    double __reloadCooldown, __currentReloadCooldown;
    double __fireCooldown  , __currentFireCooldown;
    double __recoil;
    double __fireDist;

    size_t __magazineSize;
    size_t __currentAmmo;
    size_t __totalAmmo;

    bool   __releasedTrigger;

    WeaponType  __type;
    std::string __name;

    Animation   __shootAnimation;
    Animation   __moveAnimation;
    Animation   __reloadAnimation;
    Animation   __idleAnimation;
};

#endif // WEAPON_HPP_INCLUDED
