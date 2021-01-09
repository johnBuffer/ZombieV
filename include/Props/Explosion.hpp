#ifndef EXPLOSION_HPP_INCLUDED
#define EXPLOSION_HPP_INCLUDED

#include "System/StandardEntity.hpp"
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

class Explosion : public StandardEntity<Explosion>
{
public:
    Explosion();
    Explosion(float x, float y, float angle, float openAngle, float speed, float size, size_t n);

    bool isDone()  const {return _ratio==0;}
    bool isTrace() const {return _isTrace;}

    void initPhysics(GameWorld* world) {}
    void update(GameWorld& world);
    void render();
    void setTrace(bool isTrace) {_isTrace  = isTrace;}
    void setDecrease(float d)   {_decrease = d;}
    void setSpeed(int32_t d)      { _max_speed = d;}
    void kill(GameWorld* world);

    static void init();

private:
    size_t _n;
    float  _ratio;
    float  _decrease;
    float  _openAngle;
    float  _angle;
	int32_t    _max_speed;
    float  _size;

    bool   _isTrace;
    bool   _traceOnEnd;

    std::vector<Particle>  _particles;
    sf::VertexArray _vertexArray;

    static size_t _textureID;
    static std::vector<float> _preCalculatedVx;
    static std::vector<float> _preCalculatedVy;

    static float getRandVx(int i);
    static float getRandVy(int i);

};

#endif // EXPLOSION_HPP_INCLUDED
