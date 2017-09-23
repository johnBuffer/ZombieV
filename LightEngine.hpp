#ifndef LIGHTENGINE_HPP_INCLUDED
#define LIGHTENGINE_HPP_INCLUDED

#include "WorldEntity.hpp"
#include "Config.hpp"

#include <SFML/Graphics.hpp>
#include <list>

struct ShadowCaster
{
    ShadowCaster(const Vec2& pos, float r) :
        position(pos),
        radius(r) {}

    Vec2 position;
    float radius;
};

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

    size_t _quality;
};

#endif // LIGHTENGINE_HPP_INCLUDED
