#ifndef SMOKE_HPP_INCLUDED
#define SMOKE_HPP_INCLUDED

#include "../WorldEntity.hpp"

#include <SFML/Graphics.hpp>

class Smoke : public WorldEntity
{
public:
    Smoke();
    Smoke(Vec2 pos, Vec2 vel, float angularVel, float size);

    void initPhysics(GameWorld*) {}
    void setRatioSpeed(float speed) {_ratioSpeed=speed;}
    void update(GameWorld& world);
    void render();

    static void init();

    bool isDone() const {return _isDone;}

private:
    float _ratio;
    float _ratioSpeed;
    float _size;
    float _angularVelocity;
    Vec2  _velocity;

    bool  _isDone;

    sf::VertexArray _vertexArray;
    static size_t   _textureID;


};

#endif // SMOKE_HPP_INCLUDED
