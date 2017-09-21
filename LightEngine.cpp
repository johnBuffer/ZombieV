#include "LightEngine.hpp"
#include "GameRender.hpp"
#include "LightUtils.hpp"

#include <iostream>

LightEngine::LightEngine() :
    _quality(32)
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

}

sf::Sprite LightEngine::render()
{
    sf::Color ambientLight(20, 20, 40);
    _texture.clear(ambientLight);

    std::list<Vec2> casters = GameRender::getScreenSpaceShadowCasters();

    // Draw durables lights
    for (const Light& light : _durableLights)
    {
        if (GameRender::isVisible(light.position->getCoord(), light.radius))
        {
            _interTexture.clear(sf::Color::Black);
            drawLight(light, _interTexture);

            for (const Vec2& v : casters)
            {
                Vec2 lightToCaster = v-light.position->getCoord();
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
                    Vec2 nrmLightToCaster(lightToCaster.x/dist, lightToCaster.y/dist);
                    Vec2 normal(-shadowScale*CELL_SIZE*nrmLightToCaster.y, shadowScale*CELL_SIZE*nrmLightToCaster.x);

                    float normalFactor = 0.5*light.radius/dist;

                    float midPointX = v.x + nrmLightToCaster.x*shadowLength;
                    float midPointY = v.y + nrmLightToCaster.y*shadowLength;

                    Vec2 midPoint(midPointX, midPointY);

                    Vec2 pt1 = v+normal;
                    Vec2 pt2 = midPoint+normal*normalFactor;
                    Vec2 pt3 = midPoint-normal*normalFactor;
                    Vec2 pt4 = v-normal;

                    sf::VertexArray shadow(sf::Quads, 4);
                    shadow[0].position = sf::Vector2f(pt1.x, pt1.y);
                    shadow[1].position = sf::Vector2f(pt2.x, pt2.y);
                    shadow[2].position = sf::Vector2f(pt3.x, pt3.y);
                    shadow[3].position = sf::Vector2f(pt4.x, pt4.y);

                    shadow[0].color = sf::Color::Black;
                    shadow[1].color = sf::Color::Black;
                    shadow[2].color = sf::Color::Black;
                    shadow[3].color = sf::Color::Black;

                    GameRender::renderVertexArray(shadow, _interTexture);
                }
            }

            _interTexture.display();
            _texture.draw(sf::Sprite(_interTexture.getTexture()), sf::BlendAdd);
        }
    }

    _texture.display();

    return sf::Sprite(GameRender::getBlur(_texture.getTexture()));
}
