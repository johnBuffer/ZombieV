#ifndef EXPLOSION_HPP_INCLUDED
#define EXPLOSION_HPP_INCLUDED

#include "../WorldEntity.hpp"
#include "../DrawableEntity.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

struct Particle
{
    Particle();

    void update();

    float _x , _y;
    float _vx, _vy;
    float _vax, _vay;

    float _size;
    float _speed;

    sf::Color _color;
};

class Explosion : public WorldEntity, public DrawableEntity<Explosion>
{
public:
    Explosion();
    Explosion(float x, float y, float angle, float openAngle, float speed, float size, size_t n);

    bool isDone()  const {return _ratio<0;}
    bool isTrace() const {return _isTrace;}

    void initPhysics(GameWorld* world) {}
    void update(GameWorld& world);
    void render();
    void setTrace(bool isTrace) {_isTrace  = isTrace;}
    void setDecrease(float d)   {_decrease = d;}
    void setSpeed(float d)      {_speed    = d;}

    static void init();

private:
    size_t _n;
    float  _ratio;
    float  _decrease;
    float  _openAngle;
    float  _angle;
    int    _speed;
    float  _size;

    bool   _isTrace;
    bool   _traceOnEnd;

    std::vector<Particle>  _particles;
    sf::VertexArray _vertexArray;

    static size_t _textureID;

};

#endif // EXPLOSION_HPP_INCLUDED
