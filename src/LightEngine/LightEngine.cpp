#include "LightEngine.hpp"
#include "GameRender.hpp"
#include "LightUtils.hpp"

#include <iostream>

LightEngine::LightEngine() :
    _quality(8)
{

}

void LightEngine::init(size_t width, size_t height)
{
    float factor = 0.50f;
    _texture.create(width*factor, height*factor);
    _interTexture.create(width*factor, height*factor);
}

void LightEngine::clear()
{
    sf::Color ambientLight(20, 20, 40);
    _texture.clear(ambientLight);
}

Light* LightEngine::addDurableLight(const Light& light)
{
    _durableLights.push_back(light);
    return &_durableLights.back();
}

void LightEngine::addTempLight(const Light& light)
{
    drawLight(light, _texture);
}

void LightEngine::remove(Light* light)
{
    _durableLights.remove_if([=](const Light& l){return &l == light;});
}

sf::RenderTexture& LightEngine::getTexture()
{
    return _texture;
}

sf::Sprite LightEngine::render()
{
    sf::Color ambientLight(20, 20, 40);

    const std::list<ShadowCaster>& casters(GameRender::getScreenSpaceShadowCasters());
    size_t nCasters = casters.size();

    // Draw durables lights

    int nLights = 0;
    for (const Light& light : _durableLights)
    {
        if (GameRender::isVisible(light.position, light.radius))
        {
            nLights++;
            sf::VertexArray shadows(sf::Quads, nCasters*4);
            _interTexture.clear(sf::Color::Black);
            drawLight(light, _interTexture);

            size_t currentCasterRank = 0;
            bool mustDrawShadows = true;
            for (const ShadowCaster& sc : casters)
            {
                bool occultLight = sc.drawShadow(light, shadows, currentCasterRank);
                if (occultLight)
                {
                    mustDrawShadows = false;
                    _interTexture.clear(sf::Color::Black);
                    break;
                }
                ++currentCasterRank;
            }

            if (mustDrawShadows)
            {
                sf::RenderStates states;
                states.transform.scale(0.5f, 0.5f);
                GameRender::renderVertexArray(shadows, _interTexture, states);
            }

            _interTexture.display();
            _texture.draw(sf::Sprite(_interTexture.getTexture()), sf::BlendAdd);
        }
    }

    _texture.display();
    sf::Sprite sprite(GameRender::getBlur(_texture.getTexture()));
    sprite.setScale(2.0f, 2.0f);

    return sprite;
}
