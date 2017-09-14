#ifndef LIGHTENGINE_HPP_INCLUDED
#define LIGHTENGINE_HPP_INCLUDED

#include "GameRender.hpp"
#include "WorldEntity.hpp"

struct Light
{
    Vec2      position;
    sf::Color color;
    float     radius;
    float     intensity;
};

class LightEngine
{
public:
    LightEngine();


private:
    std::list<Light> _lights;
};

#endif // LIGHTENGINE_HPP_INCLUDED
