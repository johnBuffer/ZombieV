#ifndef ZOMBIE_HPP_INCLUDED
#define ZOMBIE_HPP_INCLUDED

#include "System/StandardEntity.hpp"
#include "System/LifeForm.hpp"
#include "System/DrawableEntity.hpp"
#include "System/ChainedObject.hpp"
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
    Zombie(float x, float y);
    ~Zombie();

    void initPhysics(GameWorld* world);
    void setTarget(WorldEntity* target);
    void hit(WorldEntity* entity, GameWorld* gameWorld);
    void update(GameWorld& world);
    void render();
    void setMarked(bool b) {_marked = b;}
    void kill();

    bool isDone() const {return _done;}
    bool isMarked() const {return _marked;}

    static void init();

private:
    float _pv;
    float _speed;
    float _dmg;
    bool  _marked;
    bool  _done;

    ZombieState _currentState;

    sf::Color    _color;
    WorldEntity* _target;
    Animation    _currentAnimation;

    sf::VertexArray  _vertexArray;
    static size_t    _moveTextureID;
    static size_t    _attackTextureID;

    static Animation _moveAnimation;
    static Animation _attackAnimation;
};

#endif // ZOMBIE_HPP_INCLUDED
