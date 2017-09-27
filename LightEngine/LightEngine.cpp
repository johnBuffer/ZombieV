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
    _texture.create(width, height);
    _interTexture.create(width, height);
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
    for (const Light& light : _durableLights)
    {
        if (GameRender::isVisible(light.position, light.radius))
        {
            _interTexture.clear(sf::Color::Black);
            drawLight(light, _interTexture);
            sf::VertexArray shadows(sf::Quads, nCasters*4);
            size_t currentCasterRank = 0;

            for (const ShadowCaster& sc : casters)
            {
                bool occultLight = sc.drawShadow(light, shadows, currentCasterRank);
                if (occultLight)
                {
                    _interTexture.clear(sf::Color::Black);
                    break;
                }

                ++currentCasterRank;
            }

            GameRender::renderVertexArray(shadows, _interTexture);

            _interTexture.display();
            _texture.draw(sf::Sprite(_interTexture.getTexture()), sf::BlendAdd);
        }
    }

    _texture.display();

    return sf::Sprite(GameRender::getBlur(_texture.getTexture()));
}
