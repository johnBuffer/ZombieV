#ifndef WEAPON_HPP_INCLUDED
#define WEAPON_HPP_INCLUDED

#include <string>
#include <list>
#include "../Animation.hpp"
#include "Bullet.hpp"
#include "../Props/BulletShell.hpp"

class GameWorld;

class Weapon
{
public:
    Weapon();

    float getRecoil()   const {return _recoil;}
    float getFireDist() const {return _fireDist;}
    Animation& getShootAnimation() {return _shootAnimation;}
    Animation& getMoveAnimation()  {return _moveAnimation;}
    Animation& getIdleAnimation()  {return _idleAnimation;}

    virtual bool fire(GameWorld* world, WorldEntity* entity)=0;
    virtual void reload()=0;
    virtual void update()=0;

    void releaseTrigger() {_releasedTrigger=true;}

protected:
    float _damage;
    float _reloadCooldown, _currentReloadCooldown;
    float _fireCooldown  , _currentFireCooldown;
    float _recoil;
    float _fireDist;

    size_t _magazineSize;
    size_t _currentAmmo;
    size_t _totalAmmo;

    bool   _releasedTrigger;

    std::string _name;

    Animation   _shootAnimation;
    Animation   _moveAnimation;
    Animation   _reloadAnimation;
    Animation   _idleAnimation;
};

#endif // WEAPON_HPP_INCLUDED
