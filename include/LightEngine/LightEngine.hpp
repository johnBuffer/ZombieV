#ifndef LIGHTENGINE_HPP_INCLUDED
#define LIGHTENGINE_HPP_INCLUDED

#include "System/WorldEntity.hpp"
#include "System/Config.hpp"

#include <SFML/Graphics.hpp>
#include <list>

struct Light
{
    Light() : width(360), angle(0) {}
    Vec2      position;
    sf::Color color;
    float     radius;
    float     intensity;
    float     width;
    float     angle;
};

class LightEngine
{
public:
    LightEngine();

    void init(size_t width, size_t height);
    void clear();
    void addTempLight(const Light& light);
    void remove(Light* light);

    Light* addDurableLight(const Light& light);
    sf::RenderTexture& getTexture();

    sf::Sprite render();

private:
    std::list<Light> _durableLights;

    sf::RenderTexture _texture;
    sf::RenderTexture _interTexture;

    float m_quality;
    float m_upscaleFactor;
};

#endif // LIGHTENGINE_HPP_INCLUDED
