#ifndef EXPLOSION_HPP_INCLUDED
#define EXPLOSION_HPP_INCLUDED

#include "WorldEntity.hpp"
#include "MassiveDrawableEntity.hpp"
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

class Explosion : public WorldEntity, public MassiveDrawableEntity<Explosion>
{
public:
    Explosion();
    Explosion(float x, float y, float angle, float openAngle, float speed, float size, size_t n);

    bool isDone()  const {return __ratio<0;}
    bool isTrace() const {return __isTrace;}

    void update(GameWorld& world);
    void render();
    void setTrace()           {__isTrace  = true;}
    void setDecrease(float d) {__decrease = d;}
    void setSpeed(float d)    {__speed    = d;}

    static void resetVertexArray();
    static GraphicEntity subGetGraphicEntity()   {return GraphicEntity(__vertexArray     , __texture);}
    static GraphicEntity getGraphicEntityTrace() {return GraphicEntity(__vertexArrayTrace, __texture, GROUND);}
    static void          loadTexture();

private:
    size_t __n;
    float  __ratio;
    float  __decrease;
    float  __openAngle;
    float  __angle;
    int    __speed;
    float  __size;
    bool   __isTrace;


    std::vector<Particle>  __particles;
    static sf::VertexArray __vertexArrayTrace;
    static sf::Texture     __texture;

};

#endif // EXPLOSION_HPP_INCLUDED
