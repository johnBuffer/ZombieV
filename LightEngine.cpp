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
    _durableLights.remove_if([=](const Light& l){return &l == light;});
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
            sf::VertexArray shadows(sf::Quads, 0);

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
                    float invDist     = 1.0f/dist;
                    Vec2 nrmLightToCaster(lightToCaster.x*invDist, lightToCaster.y*invDist);
                    Vec2 normal(-shadowScale*CELL_SIZE*nrmLightToCaster.y, shadowScale*CELL_SIZE*nrmLightToCaster.x);

                    float normalFactor = 0.5*light.radius*invDist;
                    float midPointX = v.x + nrmLightToCaster.x*shadowLength;
                    float midPointY = v.y + nrmLightToCaster.y*shadowLength;

                    Vec2 midPoint(midPointX, midPointY);

                    Vec2 pt1 = v+normal;
                    Vec2 pt2 = midPoint+normal*normalFactor;
                    Vec2 pt3 = midPoint-normal*normalFactor;
                    Vec2 pt4 = v-normal;

                    sf::Vertex v1(sf::Vector2f(pt1.x, pt1.y), sf::Color::Black);
                    sf::Vertex v2(sf::Vector2f(pt2.x, pt2.y), sf::Color::Black);
                    sf::Vertex v3(sf::Vector2f(pt3.x, pt3.y), sf::Color::Black);
                    sf::Vertex v4(sf::Vector2f(pt4.x, pt4.y), sf::Color::Black);

                    shadows.append(v1);
                    shadows.append(v2);
                    shadows.append(v3);
                    shadows.append(v4);
                }
            }

            GameRender::renderVertexArray(shadows, _interTexture);

            _interTexture.display();
            _texture.draw(sf::Sprite(_interTexture.getTexture()), sf::BlendAdd);
        }
    }

    _texture.display();

    return sf::Sprite(GameRender::getBlur(_texture.getTexture()));
}
