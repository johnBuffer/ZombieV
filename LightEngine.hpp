#ifndef LIGHTENGINE_HPP_INCLUDED
#define LIGHTENGINE_HPP_INCLUDED

#include "WorldEntity.hpp"
#include "Config.hpp"

#include <SFML/Graphics.hpp>
#include <list>

struct Light
{
    WorldEntity* position;
    Vec2         staticPos;
    sf::Color    color;
    float        radius;
    float        intensity;
};

class LightEngine
{
public:
    LightEngine();

    void init(size_t width, size_t height);

    Light* addDurableLight(const Light& light);
    void   addTempLight(const Light& light);

    void remove(Light* light);

    sf::Sprite render();

private:
    std::list<Light> _durableLights;

    sf::RenderTexture _texture;
    sf::RenderTexture _interTexture;

    size_t _quality;
};

#endif // LIGHTENGINE_HPP_INCLUDED
