#include "Explosion.hpp"
#include "Config.hpp"

Particle::Particle()
{

}

void Particle::update()
{
    _x += _vx+rand()%4-2;
    _y += _vy+rand()%4-2;
}

sf::VertexArray Explosion::__vertexArrayTrace(sf::Quads, 0);
sf::Texture     Explosion::__texture;

Explosion::Explosion()
{

}

Explosion::Explosion(float x, float y, float openAngle, float angle, float speed, float size, size_t n):
    __n(n),
    __openAngle(openAngle),
    __angle(angle),
    __speed(speed),
    __size(size),
    __isTrace(false)
{
    __decrease = 0.1;
    __body.setPosition(U_2DCoord(x, y));
    __particles.resize(__n);

    for (size_t i(0); i<__n; ++i)
    {
        float speed = rand()%__speed;
        float a = (rand()%int(openAngle*10.f)-openAngle*5.f)/10.0;
        a *= DEGRAD;
        a += angle;

        /// todo create randangle
        float a2 = (rand()%int(3600))/10.0;
        a2 *= DEGRAD;

        __particles[i]._x = x;
        __particles[i]._y = y;
        __particles[i]._vx = speed*cos(a);
        __particles[i]._vy = speed*sin(a);
        __particles[i]._vax = cos(a2);
        __particles[i]._vay = sin(a2);
        __particles[i]._size = rand()%int(__size)+2;
        __particles[i]._color = sf::Color(50+rand()%100, 0, 0, 200);
    }

    __ratio = 1.0f;
}

void Explosion::update(GameWorld& world)
{
    for (Particle& p : __particles)
        p.update();

    __ratio -= __decrease;
}

void Explosion::resetVertexArray()
{
    __vertexArray.clear();
    __vertexArrayTrace.clear();
}


void Explosion::render()
{
    for (Particle& p : __particles)
    {
        float x = p._x;
        float y = p._y;

        float sx;
        float sy;

        if (__isTrace)
        {
            float a2 = (rand()%int(3600))/10.0;
            a2 *= DEGRAD;
            sx = 2*p._size*__ratio/2*cos(a2);
            sy = 2*p._size*__ratio/2*sin(a2);
        }
        else
        {
            sx = 2*p._size*__ratio/2*p._vax;
            sy = 2*p._size*__ratio/2*p._vay;
        }

        sf::Vertex v1(sf::Vector2f(x+sx, y+sy), p._color, sf::Vector2f(0 , 0));
        sf::Vertex v2(sf::Vector2f(x+sy, y-sx), p._color, sf::Vector2f(70, 0));
        sf::Vertex v3(sf::Vector2f(x-sx, y-sy), p._color, sf::Vector2f(70, 76));
        sf::Vertex v4(sf::Vector2f(x-sy, y+sx), p._color, sf::Vector2f(0 , 76));

        if (!__isTrace)
        {
            __vertexArray.append(v1);
            __vertexArray.append(v2);
            __vertexArray.append(v3);
            __vertexArray.append(v4);
        }
        else
        {
            __vertexArrayTrace.append(v1);
            __vertexArrayTrace.append(v2);
            __vertexArrayTrace.append(v3);
            __vertexArrayTrace.append(v4);
        }
    }
}

void Explosion::loadTexture()
{
    __texture.loadFromFile("data/textures/blood.png");
}
