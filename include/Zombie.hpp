#ifndef ZOMBIE_HPP_INCLUDED
#define ZOMBIE_HPP_INCLUDED

#include "System/StandardEntity.hpp"
#include "System/LifeForm.hpp"
#include "System/ChainedObject.hpp"
#include "System/AutoAim.hpp"
#include <memory>

class Zombie : public StandardEntity<Zombie>, public LifeForm
{
    enum ZombieState
    {
        IDLE,
        MOVING,
        ATTACKING
    };

public:
    Zombie();
    Zombie(float x, float y);
    ~Zombie();

    void initPhysics(GameWorld* world);
    void setTarget(EntityID target);
    void hit(WorldEntity* entity, GameWorld* gameWorld);
    void update(GameWorld& world);
    void render();
    void setMarked(bool b) {_marked = b;}
    void kill(GameWorld* world);

    bool isDone() const {return _done;}
    bool isMarked() const {return _marked;}

    static void initialize();

private:
    float _pv;
    float _speed;
    float _dmg;
    bool  _marked;
    bool  _done;

    ZombieState _currentState;

    AutoAim m_autoAim;

    sf::Color _color;
    EntityID  _target;
    Animation _currentAnimation;

    sf::VertexArray  _vertexArray;
    static uint64_t  _moveTextureID;
    static uint64_t  _attackTextureID;

    static Animation _moveAnimation;
    static Animation _attackAnimation;

    void _getTarget();
};

#endif // ZOMBIE_HPP_INCLUDED
