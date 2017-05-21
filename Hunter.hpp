#ifndef HUNTER_HPP_INCLUDED
#define HUNTER_HPP_INCLUDED

#include <map>

#include "LifeForm.hpp"
#include "WorldEntity.hpp"
#include "Weapon.hpp"
#include "EventManager.hpp"
#include "GraphicEntity.hpp"
#include "Animation.hpp"
#include "AK.hpp"
#include "Shotgun.hpp"
#include "Pistol.hpp"
#include "Fire.hpp"

enum HunterState
{
    IDLE,
    MOVING,
    SHOOTING
};

class Hunter : public LifeForm
{
public:
    Hunter(double x, double y);

    void updateControls(EventManager& em);

    void update(GameWorld& world);
    void render();

    std::list<GraphicEntity>& getGraphicEntity();
    HunterState getCurrentState()  const {return __state;}

private:
    std::vector<Weapon*> __weapons;

    Weapon*     __currentWeapon;
    int         __weaponRank;
    bool        __releasedWeaponSwap;

    double      __stamina;
    double      __angleTarget;
    bool        __clicking;
    HunterState __state;
    HunterState __lastState;
    Fire        __fire;

    sf::VertexArray    __vertexArray;

    std::list<GraphicEntity> __gas;

    void __changeAnimation(Animation& anim, bool wait=true);
};

#endif // HUNTER_HPP_INCLUDED
