#ifndef HUNTER_HPP_INCLUDED
#define HUNTER_HPP_INCLUDED

#include <map>

#include "DrawableEntity.hpp"
#include "LifeForm.hpp"
#include "WorldEntity.hpp"
#include "EventManager.hpp"
#include "Animation.hpp"

#include "Weapons/Weapons.hpp"
#include "LightEngine/LightEngine.hpp"

enum HunterState
{
    IDLE,
    MOVING,
    SHOOTING
};

class Hunter : public LifeForm, public DrawableEntity<Hunter>
{
public:
    Hunter(float x, float y);

    void initPhysics(GameWorld* world);
    void updateControls(const EventManager& em);
    void update(GameWorld& world);
    void render();

    HunterState getCurrentState() const {return _state;}
    bool isDone() const {return false;}
    Vec2 getShakyPos() const {return getCoord()+_shootingShake;}

    static void init();

private:
    std::vector<Weapon*> _weapons;

    Weapon*     _currentWeapon;
    int         _weaponRank;
    bool        _releasedWeaponSwap;
    bool        _clicking;
    float       _stamina;
    float       _angleTarget;
    HunterState _state;
    HunterState _lastState;

    Vec2        _shootingShake;

    sf::VertexArray _vertexArray;

    Light* _shootLight;
    Light* _flashlight;
    Light* _littleLight;

    float            _feetTime;
    static Animation _feetAnimation;
    static size_t    _feetTextureID;

    void _changeAnimation(Animation& anim, bool wait=true);
};

#endif // HUNTER_HPP_INCLUDED
