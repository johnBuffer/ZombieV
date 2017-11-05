#ifndef HUNTERBASE_HPP_INCLUDED
#define HUNTERBASE_HPP_INCLUDED

#include <map>

#include "System/LifeForm.hpp"
#include "System/Animation.hpp"

#include "Weapons/Weapons.hpp"
#include "LightEngine/LightEngine.hpp"

class HunterBase : public WorldEntity, public LifeForm
{
public:
    enum HunterState
    {
        IDLE,
        MOVING,
        SHOOTING,
        RELOADING
    };

    HunterBase();
    HunterBase(float x, float y);

    void update(GameWorld& world);
    void initPhysics(GameWorld* world);
    void render();

    HunterState getCurrentState() const {return _state;}
    bool isDone() const {return _life<=0;}

    void hit(WorldEntity* entity, GameWorld* gameWorld);

    int getLife() const {return _life;}

    static void init();

protected:
    std::vector<Weapon*> _weapons;

    Weapon*     _currentWeapon;
    int         _weaponRank;
    bool        _releasedWeaponSwap;
    bool        _clicking;
    float       _stamina;
    float       _angleTarget;
    HunterState _state;
    HunterState _lastState;
    Cooldown    _stepCooldown;

    sf::VertexArray _vertexArray;

    Light* _shootLight;
    Light* _flashlight;
    Light* _littleLight;

    float            _feetTime;
    static Animation _feetAnimation;
    static size_t    _feetTextureID;
    static std::vector<size_t> _stepSounds;

    void _changeState(HunterState state);
    void _changeAnimation(Animation& anim, bool wait=true);
};

#endif // HUNTERBASE_HPP_INCLUDED
