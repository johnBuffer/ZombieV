#ifndef WEAPON_HPP_INCLUDED
#define WEAPON_HPP_INCLUDED

#include <string>
#include <list>
#include "System/Animation.hpp"
#include "Bullet.hpp"
#include "Props/BulletShell.hpp"
#include "System/Cooldown.hpp"

class GameWorld;

class Weapon
{
public:
    Weapon();

    float getRecoil()   const {return _recoil;}
    float getFireDist() const {return _fireDist;}
    bool  isReady()     const;
    bool  isFull()      const {return _currentAmmo == _magazineSize;}
    bool  isEmpty()     const {return _totalAmmo == 0;}
    bool  isMagEmpty()  const {return _currentAmmo == 0;}
    bool  canReload()   const {return !isFull() && !isEmpty();}

    size_t getCurrentAmmo() const {return _currentAmmo;}
    size_t getTotalAmmo()   const {return _totalAmmo;}

    Animation& getShootAnimation()  {return _shootAnimation;}
    Animation& getMoveAnimation()   {return _moveAnimation;}
    Animation& getIdleAnimation()   {return _idleAnimation;}
    Animation& getReloadAnimation() {return _reloadAnimation;}

    virtual Vec2 getFireOutPosition(const WorldEntity* entity) const {return Vec2(0, 0);}
    virtual bool fire(GameWorld* world, WorldEntity* entity)=0;
    virtual void reload()=0;
    virtual void update()=0;

    void releaseTrigger() {_releasedTrigger=true;}

protected:
    float _damage;
    float _reloadCooldown, _currentReloadCooldown;
    float _recoil;
    float _fireDist;

    Cooldown m_fireCooldown;

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
