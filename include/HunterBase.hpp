#ifndef HUNTERBASE_HPP_INCLUDED
#define HUNTERBASE_HPP_INCLUDED

#include <vector>

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

    HunterBase(float x, float y);

    void update(GameWorld& world) {};
    void initPhysics(GameWorld* world);
    void render();

    HunterState getCurrentState() const {return _state;}
    bool isDone() const {return false;}

    static void init();

protected:
    std::vector<Weapon*> _weapons;

    Weapon*     _currentWeapon;
    int         _weaponRank;
    bool        _releasedWeaponSwap;
    bool        _clicking;
    float       _stamina;
    float       _angleTarget;
    Cooldown    _feetSoundCooldown;
    HunterState _state;
    HunterState _lastState;

    sf::VertexArray _vertexArray;

    Light* _shootLight;
    Light* _flashlight;
    Light* _littleLight;

    float            _feetTime;
    static Animation _feetAnimation;
    static size_t    _feetTextureID;
    static std::vector<size_t> _stepsSounds;

    void _walk(float speed);
    void _update();
    void _changeState(HunterState state);
    void _changeAnimation(Animation& anim, bool wait=true);

};

#endif // HUNTERBASE_HPP_INCLUDED
