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

Light* LightEngine::addDurableLight(const Light& light)
{
    _durableLights.push_back(light);
    return &_durableLights.back();
}

void LightEngine::remove(Light* light)
{
    _durableLights.remove_if([=](const Light& l){return &l == light;});
}

sf::Sprite LightEngine::render()
{
    sf::Color ambientLight(20, 20, 40);
    _texture.clear(ambientLight);

    const std::list<Vec2>& casters(GameRender::getScreenSpaceShadowCasters());
    size_t nCasters = casters.size();

    // Draw durables lights
    for (const Light& light : _durableLights)
    {
        if (GameRender::isVisible(light.position->getCoord(), light.radius))
        {
            _interTexture.clear(sf::Color::Black);
            drawLight(light, _interTexture);
            sf::VertexArray shadows(sf::Quads, nCasters*4);
            size_t currentCasterRank = 0;
            const Vec2& lightCoord(light.position->getCoord());

            for (const Vec2& v : casters)
            {
                Vec2 lightToCaster = v-lightCoord;
                float dist         = lightToCaster.getNorm();
                float shadowLength = light.radius-dist;

                if (dist < CELL_SIZE*0.5f)
                {
                    _interTexture.clear(sf::Color::Black);
                    break;
                }
                else if (shadowLength > 0)
                {
                    float shadowScale = 1.0f;
                    float invDist     = 1.0f/dist;
                    Vec2 nrmLightToCaster(lightToCaster.x*invDist, lightToCaster.y*invDist);
                    Vec2 normal(-shadowScale*CELL_SIZE*nrmLightToCaster.y, shadowScale*CELL_SIZE*nrmLightToCaster.x);

                    float normalFactor = 0.5*light.radius*invDist;
                    float midPointX = v.x + nrmLightToCaster.x*shadowLength;
                    float midPointY = v.y + nrmLightToCaster.y*shadowLength;

                    Vec2 midPoint(midPointX, midPointY);
                    Vec2 pt1(v.x+normal.x, v.y+normal.y);
                    Vec2 pt2(midPoint.x+normal.x*normalFactor, midPoint.y+normal.y*normalFactor);
                    Vec2 pt3(midPoint.x-normal.x*normalFactor, midPoint.y-normal.y*normalFactor);
                    Vec2 pt4(v.x-normal.x, v.y-normal.y);

                    shadows[4*currentCasterRank+0].position = sf::Vector2f(pt1.x, pt1.y);
                    shadows[4*currentCasterRank+1].position = sf::Vector2f(pt2.x, pt2.y);
                    shadows[4*currentCasterRank+2].position = sf::Vector2f(pt3.x, pt3.y);
                    shadows[4*currentCasterRank+3].position = sf::Vector2f(pt4.x, pt4.y);

                    shadows[4*currentCasterRank+0].color = sf::Color::Black;
                    shadows[4*currentCasterRank+1].color = sf::Color::Black;
                    shadows[4*currentCasterRank+2].color = sf::Color::Black;
                    shadows[4*currentCasterRank+3].color = sf::Color::Black;
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
