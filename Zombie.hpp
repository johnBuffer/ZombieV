#ifndef ZOMBIE_HPP_INCLUDED
#define ZOMBIE_HPP_INCLUDED

#include "LifeForm.hpp"
#include "DrawableEntity.hpp"
#include <memory>

class Zombie : public DrawableEntity<Zombie>, public WorldEntity
{
public:
    Zombie(float x, float y);

    void initPhysics(GameWorld* world);
    void setTarget(WorldEntity* target);
    void hit(WorldEntity* entity, GameWorld* gameWorld);
    void update(GameWorld& world);
    void render();

    bool isDone() const {return _done;}

    static void init();

private:
    float _pv;
    float _speed;
    float _dmg;

    bool   _done;

    sf::Color    _color;
    WorldEntity* _target;
    Animation    _currentAnimation;

    sf::VertexArray    _vertexArray;
    static size_t      _moveTextureID;
    static Animation   _moveAnimation;
};

#endif // ZOMBIE_HPP_INCLUDED
