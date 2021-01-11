#include "LightEngine/LightEngine.hpp"
#include "LightEngine/LightUtils.hpp"
#include "System/GameRender.hpp"

#include <iostream>

LightEngine::LightEngine() :
    m_quality(0.5f)
{
    m_upscaleFactor = 1.0f/m_quality;
}

void LightEngine::init(size_t width, size_t height)
{
	const uint32_t tex_width = static_cast<uint32_t>(width*m_quality);
	const uint32_t tex_height = static_cast<uint32_t>(height*m_quality);
    _texture.create(tex_width, tex_height);
    _interTexture.create(tex_width, tex_height);
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
    drawLight(light, m_quality, _texture);
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

    //std::cout << "Casters : " << nCasters << std::endl;

    // Draw durables lights
    int nLights = 0;
    for (const Light& light : _durableLights)
    {
        if (GameRender::isVisible(light.position, light.radius) && light.radius>1)
        {
            nLights++;
            sf::VertexArray shadows(sf::Quads, nCasters*4);
            _interTexture.clear(sf::Color::Black);
            drawLight(light, m_quality, _interTexture);

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
                states.transform.scale(m_quality, m_quality);
                GameRender::renderVertexArray(shadows, _interTexture, states);
            }

            _interTexture.display();
            _texture.draw(sf::Sprite(_interTexture.getTexture()), sf::BlendAdd);
        }
    }

    _texture.display();
    sf::Sprite sprite(GameRender::getBlur(_texture.getTexture()));
    sprite.setScale(m_upscaleFactor, m_upscaleFactor);

    return sprite;
}
