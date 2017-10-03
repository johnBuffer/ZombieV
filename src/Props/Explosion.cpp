#include "Props/Explosion.hpp"
#include "System/Config.hpp"
#include "System/GameRender.hpp"

size_t Explosion::_textureID;

Particle::Particle()
{

}

void Particle::update()
{
    _x += _vx+rand()%7-3;
    _y += _vy+rand()%7-3;
}

Explosion::Explosion() :
    _vertexArray(sf::Quads, 4)
{
}

Explosion::Explosion(float x, float y, float openAngle, float angle, float speed, float size, size_t n):
    _n(n),
    _openAngle(openAngle),
    _angle(angle),
    _speed(speed),
    _size(size),
    _isTrace(false),
    _vertexArray(sf::Quads, 4)
{
    _decrease = 0.1;
    _body.setPosition(Vec2(x, y));
    _particles.resize(_n);

    for (size_t i(0); i<_n; ++i)
    {
        float speed = rand()%_speed;
        float a = (rand()%int(openAngle*10.f)-openAngle*5.f)/10.0;
        a *= DEGRAD;
        a += angle;

        /// todo create randangle
        float a2 = (rand()%int(3600))/10.0;
        a2 *= DEGRAD;

        _particles[i]._x = x;
        _particles[i]._y = y;
        _particles[i]._vx = speed*cos(a);
        _particles[i]._vy = speed*sin(a);
        _particles[i]._vax = cos(a2);
        _particles[i]._vay = sin(a2);
        _particles[i]._size = rand()%int(_size)+2;

        int color = 50+rand()%75;
        _particles[i]._color = sf::Color(color, color, color);

    }

    _traceOnEnd   = true;
    _ratio = 1.0f;
}

void Explosion::update(GameWorld& world)
{
    for (Particle& p : _particles)
        p.update();

    _ratio -= _decrease;
}

/// Add the current explosion in the global Explosion's vertex array
void Explosion::render()
{
    for (Particle& p : _particles)
    {
        float x = p._x;
        float y = p._y;

        float sx;
        float sy;

        if (_isTrace)
        {
            float a2 = (rand()%int(3600))/10.0;
            a2 *= DEGRAD;
            sx = 2*p._size*_ratio/2*cos(a2);
            sy = 2*p._size*_ratio/2*sin(a2);
        }
        else
        {
            sx = 2*p._size*_ratio/2*p._vax;
            sy = 2*p._size*_ratio/2*p._vay;
        }

        sf::Color color = p._color;
        if (!_isTrace)
            color = sf::Color(150, 150, 150);

        sf::Vertex v1(sf::Vector2f(x+sx, y+sy), color, sf::Vector2f(0 , 0));
        sf::Vertex v2(sf::Vector2f(x+sy, y-sx), color, sf::Vector2f(70, 0));
        sf::Vertex v3(sf::Vector2f(x-sx, y-sy), color, sf::Vector2f(70, 76));
        sf::Vertex v4(sf::Vector2f(x-sy, y+sx), color, sf::Vector2f(0 , 76));

        _vertexArray[0] = v1;
        _vertexArray[1] = v2;
        _vertexArray[2] = v3;
        _vertexArray[3] = v4;

        if (!_isTrace)
        {
            if (_traceOnEnd && _ratio-5*_decrease<0.0f)
                GameRender::addQuad(_textureID, _vertexArray, RenderLayer::GROUND);
            else
                GameRender::addQuad(_textureID, _vertexArray, RenderLayer::RENDER);
        }
        else
        {
            GameRender::addQuad(_textureID, _vertexArray, RenderLayer::GROUND);
        }
    }
}

void Explosion::init()
{
    _textureID = GameRender::registerTexture("data/textures/blood.png");
}
