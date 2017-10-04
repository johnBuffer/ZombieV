#ifndef HUNTER_HPP_INCLUDED
#define HUNTER_HPP_INCLUDED

#include <map>

#include "HunterBase.hpp"
#include "System/LifeForm.hpp"
#include "System/ChainedObject.hpp"
#include "System/EventManager.hpp"
#include "System/Animation.hpp"

#include "Weapons/Weapons.hpp"
#include "LightEngine/LightEngine.hpp"

class Hunter : public HunterBase, public ChainedObject<Hunter>
{
public:
    Hunter(float x, float y);

    void updateControls(const EventManager& em);
    void update(GameWorld& world);

protected:
    bool        _releasedWeaponSwap;
    bool        _clicking;
};

#endif // HUNTER_HPP_INCLUDED
