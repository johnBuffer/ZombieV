#include "Props/Explosion.hpp"
#include "System/Config.hpp"
#include "System/GameRender.hpp"
#include "System/Utils.hpp"
#include "UnitedEngine/U_2DCollisionManager.h"

size_t Explosion::_textureID;
std::vector<float> Explosion::_preCalculatedVx;
std::vector<float> Explosion::_preCalculatedVy;

Particle::Particle()
{

}

void Particle::update()
{
    _x += _vx + getRandFloat(-3.0f, 3.0f);
    _y += _vy + getRandFloat(-3.0f, 3.0f);
}

Explosion::Explosion() :
    _vertexArray(sf::Quads, 4)
{
}

Explosion::Explosion(float x, float y, float openAngle, float angle, float speed, float size, size_t n):
    StandardEntity(x, y, angle),
    _n(n),
    _openAngle(openAngle*0.5f),
    _max_speed(static_cast<int32_t>(speed)),
    _size(size),
    _isTrace(false),
    _vertexArray(sf::Quads, 4)
{
    _decrease = 0.1f;
    _particles.resize(_n);

    for (size_t i(_n); i--;)
    {
        Particle& p = _particles[i];
        const float speed = static_cast<float>(rand()%_max_speed);
        const float a = getRandInt(-static_cast<int32_t>(openAngle), static_cast<int32_t>(openAngle))*DEGRAD+angle;

        const int indexA = rand()%1000;

        p._x = x;
        p._y = y;
        p._vx = speed*cos(a);
        p._vy = speed*sin(a);
		p._vax = getRandVx(indexA);
        p._vay = getRandVy(indexA);
        p._size = static_cast<float>(rand()%int(_size)+2);

        uint8_t color = static_cast<uint8_t>(getRandInt(50, 125));
		uint8_t alpha = static_cast<uint8_t>(getRandInt(100, 200));
        p._color = sf::Color(color, color, color, alpha);
    }

    _traceOnEnd   = true;
    _ratio = 1.0f;
}

void Explosion::kill(GameWorld* world)
{
    this->remove();
}

void Explosion::update(GameWorld& world)
{
    for (Particle& p : _particles)
        p.update();

    _ratio -= _decrease;
    _ratio = std::max(0.0f, _ratio);
}

/// Add the current explosion in the global Explosion's vertex array
void Explosion::render()
{
    if (_ratio > 0)
    {
        for (Particle& p : _particles)
        {
            float x = p._x;
            float y = p._y;

            float sx;
            float sy;

            if (_isTrace)
            {
                int indexA = static_cast<int32_t>(getRandInt(0, 999));
                sx = p._size*_ratio*getRandVx(indexA);
                sy = p._size*_ratio*getRandVx(indexA);
            }
            else
            {
                sx = p._size*_ratio*p._vax;
                sy = p._size*_ratio*p._vay;
            }

            sf::Color color = p._color;
            if (!_isTrace)
            {
                color.a = 255;
            }

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
                if (_traceOnEnd && _ratio-4*_decrease<0.0f)
                {
                    GameRender::addQuad(_textureID, _vertexArray, RenderLayer::GROUND);
                }
                else
                    GameRender::addQuad(_textureID, _vertexArray, RenderLayer::RENDER);
            }
            else
            {
                GameRender::addQuad(_textureID, _vertexArray, RenderLayer::GROUND);
            }
        }
    }
}

void Explosion::init()
{
    _textureID = GameRender::registerTexture("data/textures/blood.png");

    for (int i(0); i<1000; ++i)
    {
        float a = getRandomAngle(0.0f, PI2);
        _preCalculatedVx.push_back(cos(a));
        _preCalculatedVy.push_back(sin(a));
    }
}

float Explosion::getRandVx(int i)
{
    return _preCalculatedVx[i];
}

float Explosion::getRandVy(int i)
{
    return _preCalculatedVy[i];
}
